cmake_minimum_required(VERSION 3.16)

project(tubedl)

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(tubedl
    src/parser.cpp
    src/api.cpp
    src/main.cpp
    )

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

    include(FetchContent)

    FetchContent_Declare(
        Drogon
        GIT_REPOSITORY https://github.com/drogonframework/drogon.git
        GIT_TAG cf543716ce33fce395bac3e34fc8aa4c81353783
    )

    FetchContent_Declare(
        ctre
        GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions.git
        GIT_TAG 331aebc79d715d1d0036d2d32021584eefecbcc9
        GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(Drogon ctre)

    add_library(tubelibs
    INTERFACE
    )

    target_link_libraries(tubelibs
    INTERFACE
    drogon          
    ctre    
    )

    target_link_libraries(
    tubedl
    PRIVATE
    tubelibs    
    )    

    enable_testing()
    add_subdirectory(tests)

endif()