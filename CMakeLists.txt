cmake_minimum_required(VERSION 3.10)

project(N-Puzzle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_options(-Wall -Wextra -Ofast)

include_directories(
    src/
    src/class
    src/enum
    src/exception
    src/struct
)

set(
    SOURCES
    src/Parser.class.cpp
    src/exception/Exception.class.cpp
    src/class/Puzzle.class.cpp
    src/Heuristic.class.cpp
    src/struct/Position.struct.cpp
    src/main.cpp
    src/CommandLine.class.cpp
    src/PathFinding.class.cpp
    src/Gui.class.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/libs/boost/include)
include_directories(${CMAKE_SOURCE_DIR}/libs/sfml/include)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/libs)
find_package(Boost COMPONENTS program_options REQUIRED)
find_package(SFML COMPONENTS graphics system window REQUIRED)
add_executable(n-puzzle ${SOURCES})
target_link_libraries(n-puzzle LINK_PUBLIC sfml-graphics sfml-system sfml-window ${Boost_LIBRARIES})
