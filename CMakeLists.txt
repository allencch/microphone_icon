cmake_minimum_required (VERSION 2.8.11)
project (MicrophoneIcon)
set (MicrophoneIcon_VERSION_MAJOR 1)
set (MicrophoneIcon_VERSION_MINOR 0)
set (MicrophoneIcon_VERSION_PATCH 0)

cmake_policy(SET CMP0071 NEW)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wno-deprecated-declarations")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-export-dynamic")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(./include)

find_package(PkgConfig REQUIRED)
pkg_search_module(GTK3 REQUIRED gtk+-3.0)
pkg_search_module(ALSA REQUIRED alsa)

add_definitions(${GTK3_CFLAGS})
add_definitions(${ALSA_CFLAGS})

file(GLOB microphone_icon_SRC "src/*.cpp")
add_executable(microphone_icon ${microphone_icon_SRC})
target_link_libraries(microphone_icon ${GTK3_LIBRARIES} ${ALSA_LIBRARIES})

install(TARGETS microphone_icon
  DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES images/microphone-on.png images/microphone-off.png
  DESTINATION "/usr/share/microphone_icon")
install(FILES images/microphone-icon.png
  DESTINATION "/usr/share/pixmaps")
install(FILES ./microphone-icon.desktop
  DESTINATION "/usr/share/applications")


include(InstallRequiredSystemLibraries)
set (CPACK_PACKAGE_VERSION_MAJOR "${MicrophoneIcon_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${MicrophoneIcon_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${MicrophoneIcon_VERSION_PATCH}")
include(CPack)
