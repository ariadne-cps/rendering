add_library(RENDERING_SRC OBJECT
    figure.cpp
    default_rendering.cpp
    rendering_manager.cpp
)

target_include_directories(RENDERING_SRC PRIVATE
    "${PROJECT_SOURCE_DIR}/include/rendering"
)
