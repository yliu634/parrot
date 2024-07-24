#include "trait.hpp"

using namespace trait;

int main() {

  std::vector<u64> kvs;
  for (u64 i =0; i < 2000000; i++) {
    kvs.push_back(i);
  }
  double kLoadFactor = 85/100.0;
  chained_learned_hash_t* dp = new chained_learned_hash_t(kvs, kLoadFactor);
  //                  std::vector<uint64_t>(kvs.size(),2));
  //othello_hash_dp_t* dp = new othello_hash_dp_t(*cp);

  u64 v_;
  bool res;
  for (u64 i = 0; i < 2000000; i++) {
    dp->lookUp(i, v_);
    if (i % 100000 == 0) {
      std::cout << v_ << std::endl;
    }
  }
  return 0;

}

