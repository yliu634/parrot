#include "trait.hpp"

using namespace trait;
int main() {
  std::vector<u64> kvs;
  for (u64 i =0; i < 2000000; i++) {
    kvs.push_back(i);
  }
  double kLoadFactor = 70/100.0;
  ludo_hash_cp_t* cp = new ludo_hash_cp_t(kvs,kvs.size(),kLoadFactor);
  ludo_hash_dp_t* dp = new ludo_hash_dp_t(*cp);

  u64 v_;
  bool res;
  for (u64 i = 0; i < 2000000; i++) {
    if (!dp->lookUp(i, v_)) {
      std::cout << "No " << i;
      exit(0);
    }
    if (i % 100000 == 0) {
      std::cout << v_ << std::endl;
    }
  }
  return 0;

}

