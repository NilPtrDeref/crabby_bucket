cmake_minimum_required(VERSION 3.28)
project(crabby_bucket)

set(CMAKE_CXX_STANDARD 23)
add_executable(crabby_bucket src/main.cpp
        src/gamestate.h
        src/game.cpp
        src/settings.h
        src/engine.cpp
        src/engine.h
        src/pause.cpp
        src/button.cpp
        src/button.h
        src/menu.cpp
        src/menu.h
        src/game.h)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
target_link_libraries(crabby_bucket SDL2::SDL2 SDL2_ttf::SDL2_ttf)