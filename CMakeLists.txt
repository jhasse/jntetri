cmake_minimum_required(VERSION 3.15)

project(jntetri)

include(FetchContent)

FetchContent_Declare(spdlog
	URL https://github.com/gabime/spdlog/archive/v1.8.0.tar.gz
	URL_HASH SHA1=9b4302c3d77d7f84ab1a6547d9bcc2f849672e11)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(cereal
	URL https://github.com/USCiLab/cereal/archive/v1.3.0.tar.gz
	URL_HASH SHA1=e37634e0ee4063ac4694a65f5ae4742d164f5764)
FetchContent_MakeAvailable(cereal)

add_subdirectory(subprojects/jngl)

file(GLOB SRC src/*.cpp src/engine/*.cpp src/gui/*.cpp)
add_executable(jntetri ${SRC})
target_link_libraries(jntetri PRIVATE cereal jngl spdlog)
set_property(TARGET jntetri PROPERTY CXX_STANDARD 20)

if(MSVC)
	target_compile_definitions(jntetri PRIVATE BOOST_ALL_NO_LIB)
	add_custom_command(TARGET jntetri COMMAND ${CMAKE_COMMAND} -E copy
		${PROJECT_SOURCE_DIR}/subprojects/jngl/lib/msvc/x86_64/epoxy-0.dll
		"$<TARGET_FILE_DIR:jntetri>")
endif()
