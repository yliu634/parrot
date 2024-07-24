
#include "trait.hpp"

#include "./utils/load_data.h"

using namespace trait;

using namespace ludo;
using namespace parrot;
using namespace othello;
using namespace chained;

#define LUDO_HASH_TEST 0
#define PARROT_HASH_TEST 1
#define OTHELLO_HASH_TEST 1
#define LEARNED_HASH_TEST 1


void benchmark_ludo_mem(std::vector<u64>& kvs, 
                    const double kLoadFactor) {
  ludo_hash_cp_t* cp = new ludo_hash_cp_t(kvs,kvs.size(),kLoadFactor);
  ludo_hash_dp_t* dp = new ludo_hash_dp_t(*cp);
  std::cout << "Memory on ludo_hash_dp_t: " << dp->getMemoryCost()/(double)(1024*1024) << " MB\n";
  return;
}

void benchmark_parrot_mem(std::vector<u64>& kvs, 
                    const double kLoadFactor) {
  parrot_hash_cp_t* cp = new parrot_hash_cp_t(kvs,kvs.size(),kLoadFactor);
  parrot_hash_dp_t* dp = new parrot_hash_dp_t(*cp);
  std::cout << "Memory on parrot_hash_dp_t: " << dp->getMemoryCost()/(double)(1024*1024) << " MB\n";
  return;
}

void benchmark_learned_mem(std::vector<u64>& kvs, 
                    const double kLoadFactor) {
  chained_learned_hash_t* dp = new chained_learned_hash_t(kvs,kLoadFactor);
  std::cout << "Memory on chained_learned_dp_t: " << dp->getMemoryCost()/(double)(1024*1024) << " MB\n";
  return;
}

void benchmark_othello_mem(std::vector<u64>& kvs, 
                    const double kLoadFactor) {
  //std::vector<uint32_t> kvs32(kvs.begin(), kvs.end());
  othello_hash_cp_t* cp = new othello_hash_cp_t(kvs.size()/kLoadFactor,true,kvs,kvs);
  othello_hash_dp_t* dp = new othello_hash_dp_t(*cp);
  std::cout << "Memory on othello_hash_dp_t: " << dp->getMemoryCost()/(double)(1024*1024) << " MB\n";
  return;
}


int main() {

  const size_t n = 64e6;
  std::vector<size_t> ns = {16,32,48,64,80,96};
  
  std::vector<u64> kvs;

  for (size_t m = 0; m < ns.size(); m++){

    //std::vector<int> kLoads = {70,75,80,85,90,95};
    std::vector<int> kLoads = {85};
    std::vector<data_t> datasets = { data_t::WIKI, data_t::FB, data_t::OSM, data_t::BOOK};
    
    for (size_t i = 0; i < datasets.size(); i++) {

      kvs.clear();
      load_data<u64>(kvs, ns[m]*1000000, datasets[i]);

      for (size_t j = 0; j < kLoads.size(); j++) {

        #if LUDO_HASH_TEST
          std::cout << "\nBenchmark ludo_hash_dp_t with load factor: "<< kLoads[j]<< ", and datasets: "<< (int)datasets[i]<< ".."<< std::endl; 
          benchmark_ludo_mem(kvs, kLoads[j]/100.0);
        #endif

        #if PARROT_HASH_TEST
          std::cout << "\nBenchmark parrot_hash_dp_t with load factor: "<< kLoads[j]<< ", and datasets: "<< (int)datasets[i]<< ".."<< std::endl; 
          benchmark_parrot_mem(kvs, kLoads[j]/100.0);
        #endif

        #if LEARNED_HASH_TEST
          std::cout << "\nBenchmark learned_hash_dp_t with load factor: "<< kLoads[j]<< ", and datasets: "<< (int)datasets[i]<< ".."<< std::endl; 
          benchmark_learned_mem(kvs, kLoads[j]/100.0);
        #endif

        #if OTHELLO_HASH_TEST
          std::cout << "\nBenchmark othello_hash_dp_t with load factor: "<< kLoads[j]<< ", and datasets: "<< (int)datasets[i]<< ".."<< std::endl; 
          benchmark_othello_mem(kvs, kLoads[j]/100.0);
        #endif

      }
      
    }
  }
  
  std::cout << "\ndone.." << std::endl;
  return 0;


}



