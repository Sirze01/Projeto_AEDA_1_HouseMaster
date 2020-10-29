# Use C++11
set(CMAKE_CXX_STANDARD 11)
# Require (at least) it
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Don't use e.g. GNU extension (like -std=gnu++11) for portability
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_minimum_required(VERSION 3.12.4)


if (WIN32)
    link_libraries(wsock32 ws2_32)
    set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32")
    SET(CMAKE_CXX_FLAGS -Wcpp)
endif (WIN32)

include_directories(../src/)

add_library(Projeto_AEDA_1_HouseMaster main.cpp HouseMaster.cpp HouseMaster.h Individual.cpp Individual.h Services.cpp Services.h)

add_executable(application main.cpp HouseMaster.cpp HouseMaster.h Individual.cpp Individual.h Services.cpp Services.h)

target_include_directories(Projeto_AEDA_1_HouseMaster PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Projeto_AEDA_1_HouseMaster PUBLIC gtest_main coverage_config)

