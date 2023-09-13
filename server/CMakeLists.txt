cmake_minimum_required(VERSION 3.10)
project(jntetri-server)

if(NOT MSVC)
string(APPEND CMAKE_CXX_FLAGS -fdiagnostics-color)
endif()

include(../subprojects/jngl/cmake/CPM.cmake)

find_package(Boost COMPONENTS coroutine system thread)
if(NOT Boost_FOUND)
	CPMAddPackage(
		NAME Boost
		VERSION 1.77.0
		GITHUB_REPOSITORY "boostorg/boost"
		GIT_TAG "boost-1.77.0"
	)
endif()
CPMAddPackage(
	NAME nlohmann_json
	URL "https://github.com/nlohmann/json/archive/refs/tags/v3.10.2.tar.gz"
)
CPMAddPackage("gh:gabime/spdlog@1.8.2")

find_package(Threads REQUIRED)
file(GLOB SRC *.cpp)
add_executable(jntetri-server ${SRC})
target_link_libraries(jntetri-server PRIVATE
	Boost::coroutine
	Boost::system
	Threads::Threads
	nlohmann_json::nlohmann_json
	soci_core
	soci_sqlite3
	spdlog
)
target_include_directories(jntetri-server PRIVATE /usr/include/soci) # TODO: This seems wrong
target_compile_features(jntetri-server PRIVATE cxx_std_17)
