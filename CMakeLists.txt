cmake_minimum_required (VERSION 2.8.11)
project (MicrophoneIcon)

cmake_policy(SET CMP0071 NEW)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wno-deprecated-declarations")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-export-dynamic")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(./include)

find_package(PkgConfig REQUIRED)
pkg_search_module(GTK3 REQUIRED gtk+-3.0)

add_definitions(${GTK3_CFLAGS})

file(GLOB microphone_icon_SRC "src/*.cpp")
add_executable(microphone_icon ${microphone_icon_SRC})
target_link_libraries(microphone_icon ${GTK3_LIBRARIES})
