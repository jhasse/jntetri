cmake_minimum_required(VERSION 3.10)
project(jntetri-server)

include(cmake/CPM.cmake)

CPMAddPackage("gh:Orphis/boost-cmake#7f97a08b64bd5d2e53e932ddf80c40544cf45edf@1.71.0")
CPMAddPackage(
	NAME nlohmann_json
	URL "https://github.com/nlohmann/json/archive/refs/tags/v3.10.2.tar.gz"
)

find_package(Threads REQUIRED)
file(GLOB SRC *.cpp)
add_executable(jntetri-server ${SRC})
target_link_libraries(jntetri-server PRIVATE Boost::system Threads::Threads nlohmann_json::nlohmann_json) # soci_core
target_compile_features(jntetri-server PRIVATE cxx_std_17)
