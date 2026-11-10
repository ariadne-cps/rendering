cmake_minimum_required(VERSION 3.15)
project(AriadneRenderingTutorial LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
find_package(AriadneRendering REQUIRED)

add_executable(tutorial_rendering tutorial_rendering.cpp)
target_link_libraries(tutorial_rendering PRIVATE
    AriadneRendering::ariadne-rendering)
