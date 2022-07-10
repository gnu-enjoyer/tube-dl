cmake_minimum_required(VERSION 3.16)

project(tubedl-tests)

set(CMAKE_CXX_STANDARD 20)

add_executable(
    tubedl-tests
    ${CMAKE_SOURCE_DIR}/src/api.cpp
    tests.cpp
    )

include(FetchContent)

FetchContent_Declare(
    nanobench
    GIT_REPOSITORY https://github.com/martinus/nanobench.git
    GIT_TAG 26a9c089ca54927c1658c046749ca1b8fd60422c
    GIT_SHALLOW TRUE
    )

FetchContent_MakeAvailable(nanobench)

target_link_libraries(    
    tubedl-tests
    PRIVATE
    tubelibs
    nanobench    
    )   

target_include_directories(
    tubedl-tests
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    )    

ParseAndAddDrogonTests(tubedl-tests)

add_test(
    NAME tubedl
    COMMAND tubedl-tests
    )