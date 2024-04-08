#pragma once

#include <iostream>
#include <cstdint>
#include <array>
#include <ctime>
#include <mutex>
#include <chrono>
#include <memory>
#include <vector>
#include <atomic>
#include <future>
#include <thread>
#include <climits>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <utility>
#include <iterator>
#include <algorithm>
#include <unordered_map>
#include <immintrin.h>

#include "./ludo/ludo_cp_hashing.hpp"
#include "./ludo/ludo_dp_hashing.hpp"
#include "./ludo/ludo_hashing_with_fail_stats.hpp"
#include "./othello/othello_hashing.hpp"
#include "./chained/chained_learned_hashing.hpp"
#include "./parrot/parrot_hashing.hpp"

using namespace othello;
using namespace chained;
using namespace parrot;
using namespace ludo;

namespace trait {

enum data_t {
  SEQ, WIKI, FB, OSM, BOOK
};

using u64 = uint64_t;
using u32 = uint32_t;

using ludo_hash_cp_t = ControlPlaneLudo<u64,u64>;
using ludo_hash_dp_t = DataPlaneLudo<u64,u64>;
using ludo_hash_stats_t = ludostats::ControlPlaneLudo<u64,u64>;

using chained_learned_hash_t = KapilChainedModelHashTable<u64,u64,4>;

using parrot_hash_cp_t = ParrotControlPlane<u64,u64,MonotoneRMIHash<u64,10000>>;
using parrot_hash_dp_t = ParrotDataPlane<u64,u64,MonotoneRMIHash<u64,10000>>;

using othello_hash_cp_t = othello::ControlPlaneOthello<u64,u64,60,0,true,false,true>;
using othello_hash_dp_t  = othello::DataPlaneOthello<u64,u64,60>;

}






