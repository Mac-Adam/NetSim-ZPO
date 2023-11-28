cmake_minimum_required(VERSION 3.25)
project(NetSim_ZPO)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors -Wconversion)

include_directories(
        include
)

set(SOURCE_FILES
        src/package.cpp
        )

add_executable(tsp ${SOURCE_FILES} main.cpp)