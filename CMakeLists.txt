cmake_minimum_required(VERSION 3.22)
project(ydotool++ CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -Werror -Wno-unused-parameter -Wno-missing-field-initializers -Wno-unused-variable -D_GLIBCXX_DEBUG -fsanitize=undefined ")
#-fsanitize=address -Og")
#set(CMAKE_LINKER_FLAGS "-fsanitize=address")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
	argparse
	GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)

# Include GNU install directory module to detect where to install
# files on Linux/Unix systems (e.g., lib vs lib64)
include(GNUInstallDirs)

execute_process(COMMAND git describe --tags --long --always RESULT_VARIABLE RC_GIT_VER OUTPUT_VARIABLE GIT_VERSION)

if (${RC_GIT_VER} EQUAL 0)
    string(STRIP ${GIT_VERSION} GIT_VERSION)
    message("-- Version: " ${GIT_VERSION})
    add_definitions(-DVERSION=\"${GIT_VERSION}\")
endif()

set(SOURCE_FILES_DAEMON Daemon/ydotoold.cpp)
set(SOURCE_FILES_CLIENT Client/ydotool.cpp Client/tool_click.cpp Client/tool_mousemove.cpp Client/tool_type.cpp Client/tool_key.cpp Client/tool_stdin.cpp)

add_executable(ydotool++d ${SOURCE_FILES_DAEMON})
set_target_properties(ydotool++d PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(ydotool++d argparse)
install(TARGETS ydotool++d DESTINATION ${CMAKE_INSTALL_BINDIR})

add_executable(ydotool ${SOURCE_FILES_CLIENT})
set_target_properties(ydotool PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(ydotool argparse)
install(TARGETS ydotool DESTINATION ${CMAKE_INSTALL_BINDIR})

add_subdirectory(Daemon)
add_subdirectory(manpage)
