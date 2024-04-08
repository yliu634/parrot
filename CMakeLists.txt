cmake_minimum_required(VERSION 3.19)

project(fib VERSION 1.0)

include(FetchContent)
include_directories("./")
include_directories("./utils")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lstdc++fs -g -O1 -Wno-unused-result -Wno-unused-function")

set(LOG_SRC
    "utils/ludo/farmhash.cc"
    "utils/ludo/hashutil.cc"
)

set(LOG_LIBRARIES
    absl
    gflags
    stdc++fs
    boost_system
    boost_coroutine
)


file(GLOB bench_SORUCES ""  "./benchs/bench.cc"  ${LOG_SRC})
add_executable(bench ${bench_SORUCES})
set_target_properties(bench PROPERTIES RUNTIME_OUTPUT_DIRECTORY ./benchs)
target_link_libraries(bench ${LOG_LIBRARIES})

file(GLOB mem_SORUCES ""  "./benchs/mem.cc"  ${LOG_SRC})
add_executable(mem ${mem_SORUCES})
set_target_properties(mem PROPERTIES RUNTIME_OUTPUT_DIRECTORY ./benchs)
target_link_libraries(mem ${LOG_LIBRARIES})

file(GLOB insert_SORUCES ""  "./benchs/bench_insert.cc"  ${LOG_SRC})
add_executable(insert ${insert_SORUCES})
set_target_properties(insert PROPERTIES RUNTIME_OUTPUT_DIRECTORY ./benchs)
target_link_libraries(insert ${LOG_LIBRARIES})


