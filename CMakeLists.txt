cmake_minimum_required(VERSION 3.22)
project(stochasticPerfectHash)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        sph.cpp)

add_executable(sph ${SOURCE_FILES})