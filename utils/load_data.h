#pragma once

#include "trait.hpp"

using namespace std;
using namespace trait;

#define BUF_SIZE 2
void read_file_data(std::vector<uint64_t>& exist_keys, 
                    const size_t n, 
                    const std::string path) {
  std::vector<uint64_t> vec;
  FILE *fin = fopen(path.c_str(), "rb");
  if (fin == nullptr) {
      std::cerr << "No dataset file found, please run bench under main folder.." << path << std::endl;
      return;
  }
  uint64_t item_num(0);
  uint64_t buf[BUF_SIZE];
  fseek(fin,sizeof(uint64_t),SEEK_SET);
  while (item_num < n) {
    size_t num_read = fread(buf, sizeof(uint64_t), BUF_SIZE, fin);
    for (size_t i = 0; i < num_read; i++) {
      vec.push_back(buf[i]);
      item_num += 1;
    }
    if (num_read < BUF_SIZE) break;
  }
  exist_keys = std::move(vec);
  std::sort(exist_keys.begin(), exist_keys.end());
  std::cout << "Load #keys: " << exist_keys.size() << std::endl;
  fclose(fin);
  return;
}

void read_file_data_with_no_duplicate(
                    std::vector<uint64_t>& exist_keys, 
                    const size_t n, 
                    const std::string path) {
  std::vector<uint64_t> vec;
  FILE *fin = fopen(path.c_str(), "rb");
  uint64_t item_num(0);
  int64_t buf[BUF_SIZE];
  fseek(fin,sizeof(uint64_t),SEEK_SET);
  while (item_num < 200e6) {
    size_t num_read = fread(buf, sizeof(uint64_t), BUF_SIZE, fin);
    for (size_t i = 0; i < num_read; i++) {
      vec.push_back(buf[i]);
      item_num += 1;
    }
    if (num_read < BUF_SIZE) break;
  }
  exist_keys = std::move(vec);
  std::sort(exist_keys.begin(), exist_keys.end());
  auto last = std::unique(exist_keys.begin(), exist_keys.end());
  exist_keys.erase(last, exist_keys.end());
  if (exist_keys.size() > n) {
    exist_keys.resize(n);
    exist_keys.shrink_to_fit();
  }
  fclose(fin);
  std::cout << "Load #keys: " << exist_keys.size() << std::endl;
  
  return;
}

double harmonic_number(size_t n, double theta) {
    double h = 0.0;
    for (size_t i = 1; i <= n; ++i) {
        h += 1.0 / std::pow(i, theta);
    }
    return h;
}

void load_bench_data(uint64_t* bench_data, size_t nBench, std::vector<uint64_t>& kvs, bool uniform = true) {
  //bench_data = reinterpret_cast<uint64_t*>(std::malloc(nBench * sizeof(uint64_t)));
  size_t n = kvs.size();
  std::random_device rd;
  std::mt19937 gen(rd());
  if (uniform) {
    std::uniform_int_distribution<size_t> dist(0, n-1);
    for (size_t i = 0; i < nBench; ++i) {
        size_t index = dist(gen) % n;
        // Assign the value at the random index to bench_data
        bench_data[i] = kvs[index];
    }
  } else {
    size_t N = kvs.size();
    double theta = 0.9; // Zipfian parameter
    double mu_zipfian = harmonic_number(N, theta);
    double sigma_zipfian = std::sqrt(harmonic_number(N, theta) * (N - 1) * N / ((N - 2) * theta * theta));
    double p = mu_zipfian / (mu_zipfian + sigma_zipfian * sigma_zipfian);
    double r = mu_zipfian * (1 - p) / p;
    std::negative_binomial_distribution<size_t> dist(r, p);
    for (size_t i = 0; i < nBench; ++i) {
        size_t index = dist(gen) % n;
        // Assign the value at the random index to bench_data
        bench_data[i] = kvs[index];
    }
  }
  return;
}

std::vector<std::string> paths ={
  "",
  "./datasets/wiki_ts_200M_uint64",
  "./datasets/fb_200M_uint64",
  "./datasets/osm_cellids_200M_uint64",
  "./datasets/books_200M_uint64"
};

void load_insert_data(uint64_t* bench_data, size_t nBench, data_t dataset, uint64_t nSeek = 50e6) {
  if (dataset < data_t::WIKI) {
    for (uint64_t i = 0; i < nBench; i++)
      bench_data[i] = i + nSeek + 1;
    return;
  }
  size_t nBench_ = nBench;
  if (dataset == data_t::WIKI) {
    nSeek = 3*nSeek/2;
    nBench = 3*nBench/2;
  }
  assert(nBench + nSeek < 200e6);
  std::string path = paths[(int)dataset];
  std::vector<uint64_t> vec;
  FILE *fin = fopen(path.c_str(), "rb");
  if (fin == nullptr) {
      std::cerr << "No dataset file found, please run bench under main folder.." << path << std::endl;
      return;
  }
  uint64_t item_num(0);
  uint64_t buf[BUF_SIZE];
  //std::cout << "2: nBench: " << nBench << ", item_num: " << item_num << "\n";
  fseek(fin,(nSeek+1)*sizeof(uint64_t),SEEK_SET);
  while (item_num < nBench) {
    size_t num_read = fread(buf, sizeof(uint64_t), BUF_SIZE, fin);
    for (size_t i = 0; i < num_read; i++) {
      vec.push_back(buf[i]);
      item_num += 1;
    }
    if (num_read < BUF_SIZE) break;
  }
  std::sort(vec.begin(), vec.end());
  auto last = std::unique(vec.begin(), vec.end());
  vec.erase(last, vec.end());
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(vec.begin(), vec.end(), g);
  for (size_t i = 0; i < nBench_; i++) {
    *(bench_data + i) = vec[i];
  }
  vec.clear();
  std::cout << "Load insert #keys: " << item_num << std::endl;
  fclose(fin);
  return;
}

template<class K>
void load_data(std::vector<K>& data, const size_t n, data_t dataset) {
  switch (dataset){
    case SEQ: {
      data.resize(n);
      for (size_t i = 0; i < n; i++) data[i] = i;
      break;
    }
    case WIKI: {
      std::string path="/home/yiliu/build/parrot/datasets/wiki_ts_200M_uint64";
      read_file_data_with_no_duplicate(data, n, path);
      break;
    }
    case FB: {
      std::string path="/home/yiliu/build/parrot/datasets/fb_200M_uint64";
      read_file_data(data, n, path);
      break;
    }
    case OSM: {
      std::string path="/home/yiliu/build/parrot/datasets/osm_cellids_200M_uint64";
      read_file_data(data, n, path);
      break;
    }
    case BOOK: {
      std::string path="/home/yiliu/build/parrot/datasets/books_200M_uint64";
      read_file_data(data, n, path);
      break;
    }
    default: {
    }
  }
  return;
}







 