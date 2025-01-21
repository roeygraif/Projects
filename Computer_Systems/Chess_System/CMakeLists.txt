cmake_minimum_required(VERSION 2.8.12)

project(mtmproject1)

include_directories(${PROJECT_SOURCE_DIR})


set(MTM_FLAGS_DEBUG "-std=c99 --pedantic-errors -Wall -Werror") #when working
#set(MTM_FLAGS_DEBUG "-std=c99 -w -g")    #delete when working
#set(MTM_FLAGS_DEBUG "-w")

set(MTM_FLAGS_RELEASE "${MTM_FLAGS_DEBUG} -DNDEBUG")
set(CMAKE_C_FLAGS ${MTM_FLAGS_DEBUG})

#link_directories(.)
#add_executable(mtm_exe node.c map.c utils.c game.c tournament.c chessSystem.c)
add_executable(mtm_exe node.c map.c utils.c game.c tournament.c chessSystem.c)
