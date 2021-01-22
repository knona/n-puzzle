cmake_minimum_required(VERSION 3.10)

# set the project name
project(n-puzzle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER "clang++")
add_compile_options(-Wall -Wextra -Ofast)

set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/libs)

include_directories(src/ src/class src/enum src/exception src/struct)

set(SOURCES src/Parser.class.cpp src/exception/Exception.class.cpp src/class/Puzzle.class.cpp src/Heuristic.class.cpp src/struct/Position.struct.cpp src/main.cpp src/CommandLine.class.cpp src/PathFinding.class.cpp src/Gui.class.cpp)

set(BOOST_ROOT ${CMAKE_SOURCE_DIR}/libs/boost)
set(Boost_NO_SYSTEM_PATHS=TRUE)
# set(Boost_NO_SYSTEM_PATHS on CACHE BOOL "Do not search system for Boost")
find_package(Boost COMPONENTS program_options REQUIRED)
find_package(SFML COMPONENTS graphics system window REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
add_executable(n-puzzle ${SOURCES})
target_link_libraries(n-puzzle LINK_PUBLIC sfml-graphics sfml-system sfml-window ${Boost_LIBRARIES})



