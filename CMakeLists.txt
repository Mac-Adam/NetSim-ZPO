cmake_minimum_required(VERSION 3.25)
project(NetSim_ZPO)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors -Wconversion)

include_directories(
        include
)

set(SOURCES_FILES_TESTS
        test/test_package.cpp
        test/test_storage_types.cpp
        test/test_nodes.cpp
        test/test_Factory.cpp
        test/test_factory_io.cpp
        )

set(SOURCE_FILES
        src/package.cpp
        src/storage_types.cpp
        src/nodes.cpp
        src/helpers.cpp
        src/factory.cpp
        src/reports.cpp
        )

add_executable(NetSimTest ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/main_gtest.cpp)

target_include_directories(NetSimTest PUBLIC
        googletest-master/googlemock/include
        googletest-master/googletest/include
        mocks
        )


add_subdirectory(googletest-master)

target_link_libraries(NetSimTest gmock)


add_executable(NetSim ${SOURCE_FILES} main.cpp)

target_compile_definitions(NetSimTest PUBLIC EXERCISE_ID=EXERCISE_ID_FACTORY)