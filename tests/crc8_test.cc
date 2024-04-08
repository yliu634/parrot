
#include "trait.hpp"


using namespace trait;

int main() {
  
  CRC8FastHasher<uint64_t> cr(0);
  std::vector<uint32_t> bins(16,0);
  for (size_t i =0; i < 20000000; i++) {
    bins[cr(i)&0xf] ++;
  }
  std::cout << "the hashed elements in each bin are: \n";
  for (const auto&el:bins)
    std::cout << el << " ";
  std::cout << std::endl;

  return 0;
}

