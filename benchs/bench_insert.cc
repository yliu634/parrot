
#include "trait.hpp"

#include "./utils/load_data.h"

using namespace trait;

using namespace ludo;
using namespace parrot;
using namespace othello;
using namespace chained;

#define LUDO_HASH_TEST 0
#define PARROT_HASH_TEST 1
#define OTHELLO_HASH_TEST 0
#define LEARNED_HASH_TEST 1


template<class Table>
void release_space(void* table_) { delete static_cast<Table*>(table_); }

void benchmark_ludo_insert(std::vector<u64>& kvs, 
                    const double kLoadFactor, 
                    u64* data, 
                    const size_t nBench) {
  ludo_hash_cp_t* cp = new ludo_hash_cp_t(kvs,kvs.size(),kLoadFactor);
  ludo_hash_dp_t* dp = new ludo_hash_dp_t(*cp);
  
  auto start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < nBench-1; i++) {
    __builtin_prefetch(data+i+1, 0, 0);
    u64 key = *(data+i);
    dp->applyInsert(cp->insert(key,key).path, key);
    asm volatile("" : : : "memory");
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::nanoseconds elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
  double throughput = (nBench-1) / (elapsed_ns.count() * 1e-3);

  std::cout << "benchmark ludo insert throughput is: " << throughput << " Mops, and latency is: " << elapsed_ns.count()/(double)(nBench-1) << " ns."<< std::endl;
  std::cout << "-------------\n" << std::endl;

  return;
}

void benchmark_parrot_insert(std::vector<u64>& kvs, 
                    const double kLoadFactor, 
                    u64* data, 
                    const size_t nBench) {
  parrot_hash_cp_t* cp = new parrot_hash_cp_t(kvs,kvs.size(),kLoadFactor);
  parrot_hash_dp_t* dp = new parrot_hash_dp_t(*cp);
  release_space<parrot_hash_cp_t>(cp);
  
  auto start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < nBench-1; i++) {
    __builtin_prefetch(data+i+1, 0, 0);
    u64 key = *(data+i);
    dp->insert(key,key);
    asm volatile("" : : : "memory");
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::nanoseconds elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
  double throughput = (nBench-1) / (elapsed_ns.count() * 1e-3);

  std::cout << "benchmark parrot insert throughput is: " << throughput << " Mops, and latency is: " << elapsed_ns.count()/(double)(nBench-1) << " ns."<< std::endl;
  std::cout << "-------------\n" << std::endl;
  return;
}

void benchmark_learned_insert(std::vector<u64>& kvs, 
                    const double kLoadFactor, 
                    u64* data, 
                    const size_t nBench) {
  chained_learned_hash_t* dp = new chained_learned_hash_t(kvs,kLoadFactor);
  auto start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < nBench-1; i++) {
    __builtin_prefetch(data+i+1, 0, 0);
    u64 key = *(data+i);
    dp->insert2(key, key);
    asm volatile("" : : : "memory");
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::nanoseconds elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
  double throughput = (nBench-1) / (elapsed_ns.count() * 1e-3);

  std::cout << "benchmark learned insert throughput is: " << throughput << " Mops, and latency is: " << elapsed_ns.count()/(double)(nBench-1) << " ns."<< std::endl;
  std::cout << "-------------\n" << std::endl;
  return;
}

void benchmark_othello_insert(std::vector<u64>& kvs, 
                    const double kLoadFactor, 
                    u64* data, 
                    const size_t nBench) {
  othello_hash_cp_t* cp = new othello_hash_cp_t(kvs.size(),true,kvs,kvs);
  othello_hash_dp_t* dp = new othello_hash_dp_t(*cp);
  auto start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < nBench-1; i++) {
    __builtin_prefetch(data+i+1, 0, 0);
    u64 key = *(data+i);
    auto result = cp->insert(key,key);
    dp->fixHalfTreeByConnectedComponent(result.cc, result.xorTemplate);
    asm volatile("" : : : "memory");
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::nanoseconds elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
  double throughput = (nBench-1) / (elapsed_ns.count() * 1e-3);

  std::cout << "benchmark othello insert throughput is: " << throughput << " Mops, and latency is: " << elapsed_ns.count()/(double)(nBench-1) << " ns."<< std::endl;
  std::cout << "-------------\n" << std::endl;
  return;
}


int main() {

  const size_t n = 30e6;
  const size_t nBench = 1e4;
  std::vector<u64> kvs;
  u64* bench_data = (uint64_t*)malloc(nBench*sizeof(u64));

  // experimental settings
  std::vector<int> kLoads = {75,80,85,90,95};
  std::vector<data_t> datasets = {data_t::WIKI, data_t::FB, data_t::WIKI, data_t::OSM, data_t::BOOK};
  
  for (size_t i = 0; i < datasets.size(); i++) {

    kvs.clear();
    load_data<u64>(kvs, n, datasets[i]);
    load_insert_data(bench_data, nBench, datasets[i], n);

    for (size_t j = 0; j < kLoads.size(); j++) {

      #if LUDO_HASH_TEST
        std::cout << "\nBenchmark ludo_hash_dp_t with load factor: "<< kLoads[j]<< ", and datasets: "<< (int)datasets[i]<< ".."<< std::endl; 
        benchmark_ludo_insert(kvs, kLoads[j]/100.0, bench_data, nBench);
      #endif

      #if PARROT_HASH_TEST
        std::cout << "\nBenchmark parrot_hash_dp_t with load factor: "<< kLoads[j]<< ", and datasets: "<< (int)datasets[i]<< ".."<< std::endl; 
        benchmark_parrot_insert(kvs, kLoads[j]/100.0, bench_data, nBench);
      #endif

      #if LEARNED_HASH_TEST
        std::cout << "\nBenchmark learned_hash_dp_t with load factor: "<< kLoads[j]<< ", and datasets: "<< (int)datasets[i]<< ".."<< std::endl; 
        benchmark_learned_insert(kvs, kLoads[j]/100.0, bench_data, nBench);
      #endif

      #if OTHELLO_HASH_TEST
        std::cout << "\nBenchmark othello_hash_dp_t with load factor: "<< kLoads[j]<< ", and datasets: "<< (int)datasets[i]<< ".."<< std::endl; 
        benchmark_othello_insert(kvs, kLoads[j]/100.0, bench_data, nBench);
      #endif

    }
    
  }
  
  std::cout << "\ndone.." << std::endl;
  return 0;


}



