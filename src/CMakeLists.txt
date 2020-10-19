cmake_minimum_required(VERSION 3.17)
project(src)

set(CMAKE_CXX_STANDARD 14)

add_executable(src main.cpp Individuals.h Individuals.cpp Services.h Services.cpp HouseMaster.cpp HouseMaster.h)
