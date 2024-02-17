cmake_minimum_required(VERSION 3.28)
project(crabby_bucket)

set(CMAKE_CXX_STANDARD 23)

find_package(SDL2 REQUIRED)
include_directories(crabby_bucket ${SDL2_INCLUDE_DIRS})

add_executable(crabby_bucket main.cpp)
target_link_libraries(crabby_bucket ${SDL2_LIBRARIES})
