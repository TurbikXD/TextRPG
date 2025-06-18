cmake_minimum_required(VERSION 3.29)
project(game)

set(CMAKE_CXX_STANDARD 20)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG")
endif()

find_package(Threads REQUIRED)

add_executable(game
    main.cpp
    player.cpp
    enemy.cpp
    item.cpp
    room.cpp
    combat.cpp
    Game.cpp
)

target_link_libraries(game Threads::Threads)
