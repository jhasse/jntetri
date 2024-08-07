cmake_minimum_required(VERSION 3.15)

project(jntetri)

if(NOT MSVC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=implicit-function-declaration")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color")
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	set(USE_FLAGS "-s USE_SDL=2 -pthread -s USE_PTHREADS=1 -s USE_FREETYPE=1 -s USE_VORBIS=1 -s DISABLE_EXCEPTION_CATCHING=0")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
	string(APPEND CMAKE_EXE_LINKER_FLAGS
		" ${USE_FLAGS} -s WEBSOCKET_URL=wss://jntetri.boomshine.de -s ASSERTIONS=1 -s FULL_ES3=1 "
		"-s INITIAL_MEMORY=1024MB --pre-js ${CMAKE_CURRENT_SOURCE_DIR}/web/pre.js -lidbfs.js")
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()
if (TARGET_WASM)
	add_compile_options("SHELL:-s USE_PTHREADS")
	add_link_options("SHELL:-s USE_PTHREADS")
endif()

include(subprojects/jngl/cmake/CPM.cmake)
CPMAddPackage(
	NAME nlohmann_json
	URL "https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz"
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
find_package(Boost COMPONENTS coroutine system thread)

target_link_libraries(jntetri PRIVATE cereal jngl spdlog nlohmann_json::nlohmann_json)
set_property(TARGET jntetri PROPERTY CXX_STANDARD 20)

if(MSVC)
	target_compile_definitions(jntetri PRIVATE BOOST_ALL_NO_LIB _WIN32_WINNT=0x0601)
elseif(WIN32)
	target_link_libraries(jntetri PRIVATE ws2_32)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	target_link_options(jntetri PRIVATE --preload-file data)
endif()
