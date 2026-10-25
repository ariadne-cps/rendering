cmake_minimum_required(VERSION 3.15)
project(AriadneRenderingTutorial LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build with -DARIADNE_RENDERING_SOURCE_DIR=<repository root> to reuse
# the source-tree target, or provide an installed package in a parent build.
if(DEFINED ARIADNE_RENDERING_SOURCE_DIR)
    add_subdirectory("${ARIADNE_RENDERING_SOURCE_DIR}"
                     "${CMAKE_CURRENT_BINARY_DIR}/rendering" EXCLUDE_FROM_ALL)
endif()
if(NOT TARGET ariadne-rendering)
    message(FATAL_ERROR "The ariadne-rendering target is required. Set ARIADNE_RENDERING_SOURCE_DIR or include this tutorial in an Ariadne Rendering build.")
endif()

add_executable(tutorial_rendering tutorial_rendering.cpp)
target_link_libraries(tutorial_rendering PRIVATE ariadne-rendering rendering)
