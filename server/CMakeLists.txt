cmake_minimum_required(VERSION 3.10)
project(jntetri-server)

include(cmake/CPM.cmake)
CPMAddPackage(
  NAME boost-cmake
  GITHUB_REPOSITORY Orphis/boost-cmake
  VERSION 1.67.0
)
find_package(Threads REQUIRED)
file(GLOB SRC *.cpp)
add_executable(jntetri-server ${SRC})
target_link_libraries(jntetri-server PRIVATE Boost::system Threads::Threads) # soci_core
target_compile_features(jntetri-server PRIVATE cxx_std_17)
