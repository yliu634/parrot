#include "trait.hpp"


using namespace trait;

int main() {
  
  chained_model_hash_t kp(keys, 0.85);
  kp.insert(1,1);
  
  othello_hash_t oh(ks.size(), true, ks, vs);
  oh.insert(1,1);
  
  parrot_hash_t pt(keys, 64U, 85/100.0);
  pt.insert(1,1);

  return 0;
}