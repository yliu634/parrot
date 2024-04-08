
#include "trait.hpp"

#include "./utils/load_data.h"

using namespace trait;

using namespace ludo;
using namespace parrot;
using namespace othello;
using namespace chained;

#define LUDO_HASH_TEST 1
#define PARROT_HASH_TEST 1
#define OTHELLO_HASH_TEST 1
#define LEARNED_HASH_TEST 1

template<class Table>
void benchmark(void* table_, u64* data, const size_t nBench) {
  Table* table = static_cast<Table*>(table_);
  u64 v;
  auto start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < nBench-1; i++) {
    __builtin_prefetch(data + i + 1, 0, 0);
    table->lookUp(*(data + i), v);
    asm volatile("" : : "r,m"(v) : "memory");
    //asm volatile ("mfence" : : : "memory");
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::nanoseconds elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
  double throughput = (nBench-1) / (elapsed_ns.count() * 1e-3);

  std::cout << "benchmark throughput is: " << throughput << " Mops, and latency is: " << elapsed_ns.count()/(double)(nBench-1) << " ns."<< std::endl;
  std::cout << "-------------\n" << std::endl;
}

template<class Table>
void release_space(void* table_) { delete static_cast<Table*>(table_); }


void benchmark_ludo(std::vector<u64>& kvs, 
                    const double kLoadFactor, 
                    u64* bench_data, 
                    const size_t nBench) {
  ludo_hash_cp_t* cp = new ludo_hash_cp_t(kvs,kvs.size(),kLoadFactor);
  ludo_hash_dp_t* dp = new ludo_hash_dp_t(*cp);
  release_space<ludo_hash_cp_t>(cp);      // delete cp;
  // start benchmark ..
  benchmark<ludo_hash_dp_t>(dp,bench_data,nBench);
  return;
}

void benchmark_parrot(std::vector<u64>& kvs, 
                    const double kLoadFactor, 
                    u64* bench_data, 
                    const size_t nBench) {
  parrot_hash_cp_t* cp = new parrot_hash_cp_t(kvs,kvs.size(),kLoadFactor);
  parrot_hash_dp_t* dp = new parrot_hash_dp_t(*cp);
  release_space<parrot_hash_cp_t>(cp);
  // start benchmark ..
  benchmark<parrot_hash_dp_t>(dp,bench_data,nBench);
  return;
}

void benchmark_learned(std::vector<u64>& kvs, 
                    const double kLoadFactor, 
                    u64* bench_data, 
                    const size_t nBench) {
  chained_learned_hash_t* dp = new chained_learned_hash_t(kvs,kLoadFactor);
  // start benchmark ..
  benchmark<chained_learned_hash_t>(dp,bench_data,nBench);
  return;
}

void benchmark_othello(std::vector<u64>& kvs, 
                    const double kLoadFactor, 
                    u64* bench_data, 
                    const size_t nBench) {
  //std::vector<uint32_t> kvs32(kvs.begin(), kvs.end());
  othello_hash_cp_t* cp = new othello_hash_cp_t(kvs.size(),true,kvs,kvs);
  othello_hash_dp_t* dp = new othello_hash_dp_t(*cp);
  release_space<othello_hash_cp_t>(cp);
  // start benchmark ..
  benchmark<othello_hash_dp_t>(dp,bench_data,nBench);
  return;
}


int main() {

  const size_t n = 50e6;
  const size_t nBench = 5e6;
  std::vector<u64> kvs;
  u64* bench_data = (uint64_t*)malloc(nBench*sizeof(u64));

  // experimental settings
  std::vector<int> kLoads = {70,75,80,85,90,95};
  std::vector<data_t> datasets = {data_t::SEQ, data_t::FB, data_t::WIKI, data_t::OSM, data_t::BOOK};
  
  for (size_t i = 0; i < datasets.size(); i++) {

    kvs.clear();
    load_data<u64>(kvs, n, datasets[i]);
    load_bench_data(bench_data, nBench, kvs);

    for (size_t j = 0; j < kLoads.size(); j++) {

      #if LUDO_HASH_TEST
        std::cout << "\nBenchmark ludo_hash_dp_t with load factor: "<< kLoads[j]<< ", and datasets: "<< (int)datasets[i]<< ".."<< std::endl; 
        benchmark_ludo(kvs, kLoads[j]/100.0, bench_data, nBench);
      #endif

      #if PARROT_HASH_TEST
        std::cout << "\nBenchmark parrot_hash_dp_t with load factor: "<< kLoads[j]<< ", and datasets: "<< (int)datasets[i]<< ".."<< std::endl; 
        benchmark_parrot(kvs, kLoads[j]/100.0, bench_data, nBench);
      #endif

      #if LEARNED_HASH_TEST
        std::cout << "\nBenchmark learned_hash_dp_t with load factor: "<< kLoads[j]<< ", and datasets: "<< (int)datasets[i]<< ".."<< std::endl; 
        benchmark_learned(kvs, kLoads[j]/100.0, bench_data, nBench);
      #endif

      #if OTHELLO_HASH_TEST
        std::cout << "\nBenchmark othello_hash_dp_t with load factor: "<< kLoads[j]<< ", and datasets: "<< (int)datasets[i]<< ".."<< std::endl; 
        benchmark_othello(kvs, kLoads[j]/100.0, bench_data, nBench);
      #endif

    }
    
  }
  
  std::cout << "\ndone.." << std::endl;
  return 0;


}



