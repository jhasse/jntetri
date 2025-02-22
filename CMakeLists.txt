cmake_minimum_required(VERSION 3.15)

project(jntetri)

if(NOT MSVC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=implicit-function-declaration")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color -Wall -Wextra -Winit-self -Wpedantic -Wdelete-non-virtual-dtor")
	# -Wno-implicit-fallthrough', # cereal
	# -Wno-class-memaccess', # cereal")
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	set(USE_FLAGS "-s USE_SDL=2 -pthread -s USE_PTHREADS=1 -s USE_FREETYPE=1 -s USE_VORBIS=1 -s DISABLE_EXCEPTION_CATCHING=0")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
	string(APPEND CMAKE_EXE_LINKER_FLAGS
		" ${USE_FLAGS} -s WEBSOCKET_URL=wss://jntetri.boomshine.de -s ASSERTIONS=1 -s FULL_ES3=1 "
		"--pre-js ${CMAKE_CURRENT_SOURCE_DIR}/web/pre.js -lidbfs.js")
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

CPMAddPackage(
	NAME cereal
	URL https://github.com/USCiLab/cereal/archive/refs/tags/v1.3.2.tar.gz
	OPTIONS "SKIP_PORTABILITY_TEST ON" "JUST_INSTALL_CEREAL ON"
)

add_subdirectory(subprojects/jngl)

file(GLOB SRC src/*.cpp src/engine/*.cpp src/gui/*.cpp)
add_executable(jntetri ${SRC})
# find_package(Boost COMPONENTS coroutine system thread)
find_package(Boost REQUIRED)
# find_package(Boost CONFIG REQUIRED) Can't get this to work with custom Boost (web)

target_link_libraries(jntetri PRIVATE cereal jngl nlohmann_json::nlohmann_json Boost::boost)
set_property(TARGET jntetri PROPERTY CXX_STANDARD 20)

if(MSVC)
	target_compile_definitions(jntetri PRIVATE BOOST_ALL_NO_LIB _WIN32_WINNT=0x0601)
elseif(WIN32)
	target_link_libraries(jntetri PRIVATE ws2_32)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	target_link_options(jntetri PRIVATE --preload-file data)
endif()
