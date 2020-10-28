cmake_minimum_required(VERSION 3.12.4)
project(Projeto_AEDA_1_HouseMaster)

set(CMAKE_CXX_STANDARD 14)

include_directories(../src/)

add_library(Projeto_AEDA_1_HouseMasterlib main.cpp Individual.h Individual.cpp Services.h Services.cpp HouseMaster.cpp HouseMaster.h)

add_executable(Projeto_AEDA_1_HouseMaster main.cpp Individual.h Individual.cpp Services.h Services.cpp HouseMaster.cpp HouseMaster.h)


target_include_directories(Projeto_AEDA_1_HouseMaster PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Projeto_AEDA_1_HouseMaster PUBLIC gtest_main coverage_config)
