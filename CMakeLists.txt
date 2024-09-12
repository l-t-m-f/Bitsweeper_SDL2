cmake_minimum_required(VERSION 3.25)
project(Bitsweeper C)

set(OPTIMIZATION_LEVEL 0) # 0 - Debug, 1-2-3 - Optimization, 4 - Experimental

# Copy data
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})

# Flag groups variables
set(CUSTOM_DEBUG_FLAGS "-g -Wall -Wextra -pedantic -Wfloat-equal -Wundef -Wshadow \
    -Wpointer-arith -Wwrite-strings -Waggregate-return -Wunreachable-code \
    -Wstrict-prototypes -Wstrict-overflow=2 -Wcast-align -Wcast-qual \
     -Wconversion -Wswitch-default -Wswitch-enum")
set(CUSTOM_OPTIMIZATION_FLAGS_1 "-O1")
set(CUSTOM_OPTIMIZATION_FLAGS_2 "-O2")
set(CUSTOM_OPTIMIZATION_FLAGS_SUPPLEMENT "-march=native -funroll-loops -finline-functions")
set(CUSTOM_EXPERIMENTAL_FLAGS "-O3 -Ofast -ffast-math")
set(CUSTOM_LIBRARY_FLAGS "-fopenmp")

# Packages
# # OpenMP
find_package(OpenMP 4.5 REQUIRED)

if (OpenMP_FOUND)
    message("-- OpenMP v4.5 found!")
else ()
    message(FATAL_ERROR "-- OpenMP v4.5 not found")
endif ()

# # SDL2 and friends
find_package(SDL2 2.28.1 REQUIRED)
if (SDL2_FOUND)
    message("-- SDL2 2.28.1 found!")
else ()
    message(FATAL_ERROR "-- SDL2 2.28.1 not found")
endif ()

find_package(SDL2_image REQUIRED)
if (SDL2_image_FOUND)
    message("-- SDL2 image extension found!")
else ()
    message(FATAL_ERROR "-- SDL2 image extension not found")
endif ()

find_package(SDL2_ttf REQUIRED)
if (SDL2_ttf_FOUND)
    message("-- SDL2 ttf extension found!")
else ()
    message(FATAL_ERROR "-- SDL2 ttf extension not found")
endif ()

find_package(SDL2_mixer REQUIRED)
if (SDL2_mixer_FOUND)
    message("-- SDL2 mixer extension found!")
else ()
    message(FATAL_ERROR "-- SDL2 mixer extension not found")
endif ()

# # SDL2 FontCache
set(SDL_FontCache_INCLUDE "deps/SDL_FontCache")
if (DEFINED SDL_FontCache_INCLUDE)
    add_library(SDL_FontCache STATIC "deps/SDL_FontCache/SDL_FontCache.c")
    target_compile_options(SDL_FontCache PRIVATE)
    target_link_libraries(SDL_FontCache PUBLIC -lSDL2main -lSDL2 -lSDL2_ttf)
    target_include_directories(SDL_FontCache PRIVATE ${SDL_FontCache_INCLUDE})

    include_directories(${SDL_FontCache_INCLUDE})
    set(STATIC_LIBRARIES ${STATIC_LIBRARIES} SDL_FontCache)
    message("-- SDL_FontCache static library built with success!")
elseif ()
    message(FATAL_ERROR "-- SDL_FontCache static library was not built!")
endif ()

# Set flags according to the mode of compilation
if (OPTIMIZATION_LEVEL EQUAL 0)
    set(CMAKE_C_FLAGS_DEBUG "${CUSTOM_DEBUG_FLAGS} ${CUSTOM_LIBRARY_FLAGS}"
            CACHE STRING "C Flags" FORCE)
elseif (OPTIMIZATION_LEVEL EQUAL 1)
    set(CMAKE_C_FLAGS "${CUSTOM_OPTIMIZATION_FLAGS_1} ${CUSTOM_LIBRARY_FLAGS}"
            CACHE STRING "C Flags" FORCE)
elseif (OPTIMIZATION_LEVEL EQUAL 2)
    set(CMAKE_C_FLAGS "${CUSTOM_OPTIMIZATION_FLAGS_2} ${CUSTOM_LIBRARY_FLAGS}"
            CACHE STRING "C Flags" FORCE)
elseif (OPTIMIZATION_LEVEL EQUAL 3)
    set(CMAKE_C_FLAGS "${CUSTOM_OPTIMIZATION_FLAGS_2} \
        ${CUSTOM_OPTIMIZATION_FLAGS_SUPPLEMENT} ${CUSTOM_LIBRARY_FLAGS}"
            CACHE STRING "C Flags" FORCE)
elseif (OPTIMIZATION_LEVEL EQUAL 4)
    set(CMAKE_C_FLAGS "${CUSTOM_EXPERIMENTAL_FLAGS} \
        ${CUSTOM_OPTIMIZATION_FLAGS_SUPPLEMENT} ${CUSTOM_LIBRARY_FLAGS}"
            CACHE STRING "C Flags" FORCE)
endif ()

add_executable(${PROJECT_NAME} src/main.c src/game.c src/engine.c src/interface.c src/util.c src/bitwise.c)

set(INCLUDE "src" "src/global")
include_directories(${INCLUDE})

# Linking to Pluto
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC -OpenMP_C -lmingw32 -lSDL2main
            -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer ${STATIC_LIBRARIES})
else ()
    target_link_libraries(${PROJECT_NAME} PUBLIC -OpenMP_C -lSDL2main
            -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer ${STATIC_LIBRARIES})
endif ()

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        C_STANDARD 23
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
)
