cmake_minimum_required(VERSION 3.24)
project(Bitsweeper C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_COMPILER GCC)

set(SOURCES main.c src/bitwise.c src/game.c src/engine.c src/util.c src/interface.c)

set(CMAKE_C_FLAGS "-Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wwrite-strings -Waggregate-return -Wstrict-prototypes -Wstrict-overflow=2 -Wcast-align -Wconversion -Wswitch-default -Wswitch-enum -Wunreachable-code" CACHE STRING "C Flags" FORCE)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/gfx DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/fonts DESTINATION ${CMAKE_BINARY_DIR})

add_executable(Bitsweeper ${SOURCES} include/common.h)
target_link_libraries(${PROJECT_NAME} -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer)
