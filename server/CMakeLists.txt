cmake_minimum_required(VERSION 3.10)
project(jntetri-server)

include(cmake/CPM.cmake)

CPMAddPackage(
	NAME Boost
	VERSION 1.77.0
	GITHUB_REPOSITORY "boostorg/boost"
	GIT_TAG "boost-1.77.0"
)
CPMAddPackage(
	NAME nlohmann_json
	URL "https://github.com/nlohmann/json/archive/refs/tags/v3.10.2.tar.gz"
)

find_package(Threads REQUIRED)
file(GLOB SRC *.cpp)
add_executable(jntetri-server ${SRC})
target_link_libraries(jntetri-server PRIVATE Boost::asio Threads::Threads nlohmann_json::nlohmann_json soci_core soci_sqlite3)
target_include_directories(jntetri-server PRIVATE /usr/include/soci) # TODO: This seems wrong
target_compile_features(jntetri-server PRIVATE cxx_std_17)
