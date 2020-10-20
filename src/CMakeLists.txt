cmake_minimum_required(VERSION 3.17)
project(src)

set(CMAKE_CXX_STANDARD 14)

<<<<<<< Updated upstream
add_executable(src main.cpp Individual.h Individual.cpp Services.h Services.cpp HouseMaster.cpp HouseMaster.h)
=======
add_executable(src main.cpp Individual.h Individual.cpp Services.h Services.cpp HouseMaster.cpp HouseMaster.h Collaborator.h Collaborator.cpp Client.h Client.cpp)
>>>>>>> Stashed changes
