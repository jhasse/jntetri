cmake_minimum_required(VERSION 3.10)
project(jntetri-server)

include(cmake/CPM.cmake)
CPMAddPackage("gh:Orphis/boost-cmake#7f97a08b64bd5d2e53e932ddf80c40544cf45edf@1.71.0")
find_package(Threads REQUIRED)
file(GLOB SRC *.cpp)
add_executable(jntetri-server ${SRC})
target_link_libraries(jntetri-server PRIVATE Boost::system Threads::Threads) # soci_core
target_compile_features(jntetri-server PRIVATE cxx_std_17)
