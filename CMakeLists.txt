cmake_minimum_required(VERSION 3.15)
project(BPHOComputationalChallenge
        LANGUAGES CXX
        VERSION 1.0
)

set(CMAKE_USE_HOST_SFML OFF)
add_subdirectory(vendors)
add_subdirectory(src)