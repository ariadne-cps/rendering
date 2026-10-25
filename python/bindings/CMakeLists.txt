include_directories(${Python3_INCLUDE_DIRS})

get_target_property(PYTHON_COMMON_SOURCE_DIR pyariadne-python-common SOURCE_DIR)

add_library(pyariadne-rendering-submodule-obj OBJECT
    rendering_submodule.cpp
)
target_include_directories(pyariadne-rendering-submodule-obj PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${PYTHON_COMMON_SOURCE_DIR}/include"
)
target_link_libraries(pyariadne-rendering-submodule-obj PRIVATE
    pyariadne-python-common
    pybind11::module
    rendering
)
set_target_properties(pyariadne-rendering-submodule-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(pyariadne-rendering INTERFACE)
target_sources(pyariadne-rendering INTERFACE
    $<TARGET_OBJECTS:pyariadne-rendering-submodule-obj>
)
target_include_directories(pyariadne-rendering INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

ariadne_add_python_module(
    NAME rendering
    MODULE_SOURCE rendering_module.cpp
)
