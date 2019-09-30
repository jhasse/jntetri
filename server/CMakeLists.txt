cmake_minimum_required(VERSION 3.14)
project(jntetri-server)

find_package(Boost 1.69.0 REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)
file(GLOB SRC *.cpp)
add_executable(jntetri-server ${SRC})
target_link_libraries(jntetri-server PRIVATE ${BOOST_LIBRARIES} Threads::Threads)
