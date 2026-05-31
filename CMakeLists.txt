cmake_minimum_required(VERSION 3.27)
# Specify the languages used to optimize configuration
project(Arcade LANGUAGES CXX)

# === GLOBAL CONFIGURATION ===
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
find_package(PkgConfig QUIET)

# === INTERFACE TARGET (Common includes and flags) ===
add_library(arcadeGlobal INTERFACE)

target_compile_options(arcadeGlobal INTERFACE
    -Wall -Wextra -Werror -Wpedantic
)

target_include_directories(arcadeGlobal INTERFACE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/interface
    ${PROJECT_SOURCE_DIR}/include/core
    ${PROJECT_SOURCE_DIR}/include/module
    ${PROJECT_SOURCE_DIR}/include/module/game
    ${PROJECT_SOURCE_DIR}/include/module/graphic
    ${PROJECT_SOURCE_DIR}/include/module/menu
)

# === UTILITY FUNCTION ===
# Expected arguments:
# - PREFIX : The prefix for the .so file (e.g., "arcade_" or "")
# - SOURCES : List of source files
# - INCLUDES : (Optional) Specific include directories
# - LIBRARIES : (Optional) Specific libraries to link
function(add_arcade_module TARGET_NAME)
    cmake_parse_arguments(ARG "" "PREFIX" "SOURCES;INCLUDES;LIBRARIES" ${ARGN})

    # Create the shared library
    add_library(${TARGET_NAME} SHARED ${ARG_SOURCES})

    # Link with the global project interface
    target_link_libraries(${TARGET_NAME} PRIVATE arcadeGlobal)

    # Add specific includes if provided
    if(ARG_INCLUDES)
        target_include_directories(${TARGET_NAME} PRIVATE ${ARG_INCLUDES})
    endif()

    # Add specific libraries if provided
    if(ARG_LIBRARIES)
        target_link_libraries(${TARGET_NAME} PRIVATE ${ARG_LIBRARIES})
    endif()

    # Set properties (output directory and library prefix)
    set_target_properties(${TARGET_NAME} PROPERTIES
        PREFIX "${ARG_PREFIX}"
        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib
    )
endfunction()

# === PROJECT CORE ===
file(GLOB CORE_FILES CONFIGURE_DEPENDS "source/core/*.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
add_executable(arcade source/main.cpp ${CORE_FILES})
target_link_libraries(arcade PRIVATE arcadeGlobal ${CMAKE_DL_LIBS})

# === GAME MODULES ===

# --- MENU ---
add_arcade_module(ArcadeMenu
    PREFIX ""
    SOURCES source/module/menu/ArcadeMenu.cpp source/module/AGameModule.cpp
)

# --- SNAKE ---
add_arcade_module(snake
    PREFIX "arcade_"
    SOURCES source/module/game/SnakeGame.cpp source/module/AGameModule.cpp
)

# --- SOLARFOX ---
add_arcade_module(solarfox
    PREFIX "arcade_"
    SOURCES source/module/game/SolarfoxGame.cpp source/module/AGameModule.cpp
)

# === GRAPHIC MODULES ===

# --- SDL2 ---
# Try finding SDL2 packages on the system
set(SDL_SHARED ON CACHE BOOL "Build SDL shared library" FORCE)
set(SDL_STATIC OFF CACHE BOOL "Build SDL static library" FORCE)

find_package(SDL2 QUIET)
find_package(SDL2_image QUIET)
find_package(SDL2_ttf QUIET)

# Fallback: Fetch SDL2 from GitHub if any package is missing
if(NOT SDL2_FOUND OR NOT SDL2_image_FOUND OR NOT SDL2_ttf_FOUND)
    message(STATUS "Fetching SDL2 dependencies from GitHub...")
    fetchcontent_declare(SDL2 GIT_REPOSITORY https://github.com/libsdl-org/SDL.git GIT_TAG release-2.30.0 GIT_SHALLOW TRUE)
    fetchcontent_declare(SDL2_image GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git GIT_TAG release-2.8.2 GIT_SHALLOW TRUE)
    fetchcontent_declare(SDL2_ttf GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git GIT_TAG release-2.22.0 GIT_SHALLOW TRUE)
    fetchcontent_makeavailable(SDL2 SDL2_image SDL2_ttf)
endif()

add_arcade_module(sdl2
    PREFIX "arcade_"
    SOURCES source/module/graphic/SDL2Display.cpp source/module/ADisplayModule.cpp
    LIBRARIES SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf
)

# --- ALLEGRO 5 ---
# Try finding Allegro5 via PkgConfig
if(PKG_CONFIG_FOUND)
    pkg_check_modules(ALLEGRO5 QUIET allegro-5)
    pkg_check_modules(ALLEGRO5_IMAGE QUIET allegro_image-5)
    pkg_check_modules(ALLEGRO5_TTF QUIET allegro_ttf-5)
    pkg_check_modules(ALLEGRO5_PRIMITIVES QUIET allegro_primitives-5)
endif()

# Fallback: Fetch Allegro5 from GitHub if PkgConfig fails
if(NOT ALLEGRO5_FOUND)
    message(STATUS "Fetching Allegro 5 dependencies from GitHub...")
    set(WANT_IMAGE ON CACHE BOOL "Build image addon" FORCE)
    set(WANT_TTF ON CACHE BOOL "Build ttf addon" FORCE)
    set(WANT_PRIMITIVES ON CACHE BOOL "Build primitives addon" FORCE)
    set(SHARED ON CACHE BOOL "Build shared libraries" FORCE)

    fetchcontent_declare(allegro5 GIT_REPOSITORY https://github.com/liballeg/allegro5.git GIT_TAG 5.2.9.0 GIT_SHALLOW TRUE)
    fetchcontent_makeavailable(allegro5)

    # Map the fallback variables to match PkgConfig behavior
    set(ALLEGRO5_INCLUDE_DIRS ${allegro5_SOURCE_DIR}/include ${allegro5_BINARY_DIR}/include)
    set(ALLEGRO5_LIBRARIES allegro)
    set(ALLEGRO5_IMAGE_LIBRARIES allegro_image)
    set(ALLEGRO5_TTF_LIBRARIES allegro_ttf)
    set(ALLEGRO5_PRIMITIVES_LIBRARIES allegro_primitives)
endif()

add_arcade_module(allegro5
    PREFIX "arcade_"
    SOURCES source/module/graphic/ALLEGRO5Display.cpp source/module/ADisplayModule.cpp
    INCLUDES ${ALLEGRO5_INCLUDE_DIRS} ${ALLEGRO5_IMAGE_INCLUDE_DIRS} ${ALLEGRO5_TTF_INCLUDE_DIRS}
    LIBRARIES ${ALLEGRO5_LIBRARIES} ${ALLEGRO5_IMAGE_LIBRARIES} ${ALLEGRO5_TTF_LIBRARIES} ${ALLEGRO5_PRIMITIVES_LIBRARIES}
)

# --- NCURSES ---
find_package(Curses REQUIRED)

add_arcade_module(ncurses
    PREFIX "arcade_"
    SOURCES source/module/graphic/NCURSESDisplay.cpp source/module/ADisplayModule.cpp
    INCLUDES ${CURSES_INCLUDE_DIRS}
    LIBRARIES ${CURSES_LIBRARIES}
)

# === TESTS ===
if(PKG_CONFIG_FOUND)
    pkg_check_modules(CRITERION criterion QUIET)
endif()

if(CRITERION_FOUND)
    file(GLOB TEST_FILES CONFIGURE_DEPENDS "tests/*.cpp")
    add_executable(tests_run ${TEST_FILES})

    target_link_libraries(tests_run PRIVATE arcadeGlobal ${CRITERION_LIBRARIES})
    target_include_directories(tests_run PRIVATE ${CRITERION_INCLUDE_DIRS})

    enable_testing()
    add_test(NAME tests COMMAND tests_run)
endif()
