cmake_minimum_required(VERSION 3.15)

project(jntetri)

if(NOT MSVC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=implicit-function-declaration")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color")
endif()

include(server/cmake/CPM.cmake)
CPMAddPackage(
	NAME nlohmann_json
	URL "https://github.com/nlohmann/json/archive/refs/tags/v3.10.2.tar.gz"
)

include(FetchContent)

FetchContent_Declare(spdlog
	URL https://github.com/gabime/spdlog/archive/v1.8.0.tar.gz
	URL_HASH SHA1=9b4302c3d77d7f84ab1a6547d9bcc2f849672e11)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(cereal
	GIT_REPOSITORY https://github.com/USCiLab/cereal.git
	GIT_TAG 64f50dbd5cecdaba785217e2b0aeea3a4f1cdfab)
set(SKIP_PERFORMANCE_COMPARISON ON CACHE BOOL "Skip building performance comparison (requires boost)" FORCE)
set(SKIP_PORTABILITY_TEST ON CACHE BOOL "Skip portability (32 bit) tests" FORCE)
set(WITH_WERROR OFF CACHE BOOL "Compile with '-Werror' C++ compiler flag" FORCE)
FetchContent_MakeAvailable(cereal)

add_subdirectory(subprojects/jngl)

file(GLOB SRC src/*.cpp src/engine/*.cpp src/gui/*.cpp)
add_executable(jntetri ${SRC})
target_link_libraries(jntetri PRIVATE cereal jngl spdlog nlohmann_json::nlohmann_json)
set_property(TARGET jntetri PROPERTY CXX_STANDARD 20)

if(MSVC)
	target_compile_definitions(jntetri PRIVATE BOOST_ALL_NO_LIB)
	add_custom_command(TARGET jntetri COMMAND ${CMAKE_COMMAND} -E copy
		${PROJECT_SOURCE_DIR}/subprojects/jngl/lib/msvc/x86_64/epoxy-0.dll
		"$<TARGET_FILE_DIR:jntetri>")
endif()
