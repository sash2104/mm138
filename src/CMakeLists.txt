cmake_minimum_required (VERSION 3.13)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-O3")
# set(CMAKE_CXX_FLAGS "-pg")
# set(CMAKE_CXX_FLAGS "-g")

# set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")

set(target "run")
add_executable(${target} main.cpp)
