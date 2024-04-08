
#include "ludo_cp_hashing.hpp"

namespace ludo {

template<class K, class V, uint VL = sizeof(V) * 8>
class DataPlaneLudo : public LudoCommon<K, V, VL> {
public:
  using LudoCommon<K, V, VL>::kSlotsPerBucket;
  //using LudoCommon<K, V, VL>::kLoadFactor;
  using LudoCommon<K, V, VL>::kMaxBFSPathLen;
  
  using LudoCommon<K, V, VL>::VDMask;
  using LudoCommon<K, V, VL>::ValueMask;
  
  using LudoCommon<K, V, VL>::fast_map_to_buckets;
  using LudoCommon<K, V, VL>::multiply_high_u32;
  
  using LudoCommon<K, V, VL>::LocatorSeedLength;
  using LudoCommon<K, V, VL>::num_buckets_;
  using LudoCommon<K, V, VL>::h;
  using LudoCommon<K, V, VL>::digestH;
  
  typedef Ludo_PathEntry<K> PathEntry;
  
  struct Bucket {  // only as parameters and return values for easy access. the storage is compact.
    uint8_t seed;
    V values[kSlotsPerBucket];
    
    bool operator==(const Bucket &other) const {
      if (seed != other.seed) return false;
      for (char s = 0; s < kSlotsPerBucket; s++) {
        if (values[s] != other.values[s]) return false;
      }
      return true;
    }
    
    bool operator!=(const Bucket &other) const {
      return !(*this == other);
    }
  };
  //  static const uint8_t bucketLength = sizeof(Bucket);
  
  vector<Bucket> buckets;
  double kLoadFactor = 0.95;
  DataPlaneOthello<K, uint8_t, 1> locator;
  vector<uint8_t> lock = vector<uint8_t>(8192, 0);
  
  virtual void setSeed(uint32_t s) {
    LudoCommon<K, V, VL>::setSeed(s);
    
    locator.hab.setSeed(rand() | (uint64_t(rand()) << 32));
    locator.hd.setSeed(rand());
  }
  
  DataPlaneLudo() {}
  
  explicit DataPlaneLudo(const ControlPlaneLudo<K, V, VL> &cp) : locator(cp.locator) {
    h = cp.h;
    digestH = cp.digestH;
    kLoadFactor = cp.kLoadFactor;
    num_buckets_ = cp.num_buckets_;
    resetMemory();
    
    for (uint32_t bktIdx = 0; bktIdx < num_buckets_; ++bktIdx) {
      const typename ControlPlaneLudo<K, V, VL>::Bucket &cpBucket = cp.buckets_[bktIdx];
      Bucket &dpBucket = buckets[bktIdx];
      dpBucket.seed = cpBucket.seed;
      memset(dpBucket.values, 0, kSlotsPerBucket * sizeof(V));
      
      const FastHasher64<K> locateHash(cpBucket.seed);
      
      for (char slot = 0; slot < kSlotsPerBucket; ++slot) {
        if (cpBucket.occupiedMask & (1U << slot)) {
          const K &k = cpBucket.keys[slot];
          dpBucket.values[locateHash(k) >> 62] = cpBucket.values[slot];
        }
      }
    }
  }
  
  // will clear all entries. because it is only used at initialization
  void resizeCapacity(uint32_t targetCapacity) {
    targetCapacity = max(targetCapacity, 256U);
    
    uint64_t nextNbuckets = 64U;
    uint64_t nextCapacity = nextNbuckets * kLoadFactor * kSlotsPerBucket;
    for (; nextCapacity < targetCapacity; nextCapacity = nextNbuckets * kLoadFactor * kSlotsPerBucket)
      nextNbuckets <<= 1U;
    
    num_buckets_ = nextNbuckets;
    resetMemory();
    
    locator.resizeCapacity(nextCapacity);
  }
  
  inline void resetMemory() {
    buckets.resize(num_buckets_);
  }
  
  inline V lookUp(const K &k) const {
    V out;
    if (!lookUp(k, out)) throw runtime_error("key does not exist");
    return out;
  }
  
  // Returns true if found.  Sets *out = value.
  inline bool lookUp(const K &k, V &out) const {
    uint32_t bktId[2];
    fast_map_to_buckets(h(k), bktId, num_buckets_);
    
    while (true) {
      
      uint8_t va1 = lock[bktId[0] & 8191], vb1 = lock[bktId[1] & 8191];
      COMPILER_BARRIER();
      if (va1 % 2 == 1 || vb1 % 2 == 1) continue;
      
      const Bucket &bucket = buckets[bktId[locator.lookUp(k)]];
      
      COMPILER_BARRIER();
      uint8_t va2 = lock[bktId[0] & 8191], vb2 = lock[bktId[1] & 8191];
      
      if (va1 != va2 || vb1 != vb2) continue;
      
      uint64_t i = FastHasher64<K>(bucket.seed)(k) >> 62;
      out = bucket.values[i];
      
      return true;
    }
  }
  
  inline void applyInsert(const vector<PathEntry> &path, V value) {
    for (int i = 0; i < path.size(); ++i) {
      PathEntry entry = path[i];
      Bucket bucket = buckets[entry.bid];
      bucket.seed = entry.newSeed;
      
      uint8_t toSlots[] = {entry.s0, entry.s1, entry.s2, entry.s3};
      
      V buffer[4];       // solve the permutation is slow. just copy the 4 elements
      for (char s = 0; s < 4; ++s) {
        buffer[s] = bucket.values[s];
      }
      
      for (char s = 0; s < 4; ++s) {
        bucket.values[toSlots[s]] = buffer[s];
      }
      
      if (i + 1 == path.size()) {  // put the new value
        bucket.values[entry.sid] = value;
      } else {  // move key from another bucket and slot to this bucket and slot
        PathEntry from = path[i + 1];
        uint8_t tmp[4] = {from.s0, from.s1, from.s2, from.s3};
        uint8_t sid;
        for (uint8_t ii = 0; ii < 4; ++ii) {
          if (tmp[ii] == from.sid) {
            sid = ii;
            break;
          }
        }
        bucket.values[entry.sid] = buckets[from.bid].values[sid];
      }
      
      lock[entry.bid & 8191]++;
      COMPILER_BARRIER();
      
      if (entry.locatorCC.size()) {
        locator.fixHalfTreeByConnectedComponent(entry.locatorCC, 1);
      }
      
      buckets[entry.bid] = bucket;
      
      COMPILER_BARRIER();
      lock[entry.bid & 8191]++;
    }
  }
  
  inline void applyUpdate(uint32_t bid, uint8_t sid, V val) {
    lock[bid & 8191]++;
    COMPILER_BARRIER();
  
    buckets[bid].values[sid] = val;
    
    COMPILER_BARRIER();
    lock[bid & 8191]++;
  }
  
  inline void applyUpdate(uint32_t bs, V val) {
    uint32_t bid = bs >> 2;
    uint8_t sid = bs & 3;
    
    applyUpdate(bid, sid, val);
  }
  
  inline uint64_t getMemoryCost() const {
    return buckets.size() * sizeof(Bucket) + locator.getMemoryCost();
  }
};

}


