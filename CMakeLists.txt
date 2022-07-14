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
        simdjson
        GIT_REPOSITORY https://github.com/simdjson/simdjson.git
        GIT_TAG 1075e8609c4afa253162d441437af929c29e31bb
    )

    FetchContent_Declare(
        re2
        GIT_REPOSITORY https://github.com/google/re2.git
        GIT_TAG 0dade9ff39bb6276f18dd6d4bc12d3c20479ee24
    )

    FetchContent_Declare(
        ctre
        GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions.git
        GIT_TAG 331aebc79d715d1d0036d2d32021584eefecbcc9
        GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(Drogon simdjson re2 ctre)

    add_library(tubelibs
    INTERFACE
    )

    target_link_libraries(tubelibs
    INTERFACE
    drogon      
    simdjson
    re2    
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