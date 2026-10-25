find_library(ARIADNE_RENDERING_LIBRARY NAMES ariadne-rendering)
find_path(ARIADNE_RENDERING_INCLUDE_DIR ariadne-rendering.hpp
          PATH_SUFFIXES ariadne-rendering)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(AriadneRendering DEFAULT_MSG
    ARIADNE_RENDERING_LIBRARY
    ARIADNE_RENDERING_INCLUDE_DIR
)

if(AriadneRendering_FOUND)
    get_filename_component(ARIADNE_RENDERING_INCLUDE_PARENT_DIR
        "${ARIADNE_RENDERING_INCLUDE_DIR}" DIRECTORY)
    if(NOT TARGET AriadneRendering::ariadne-rendering)
        add_library(AriadneRendering::ariadne-rendering UNKNOWN IMPORTED)
        set_target_properties(AriadneRendering::ariadne-rendering PROPERTIES
            IMPORTED_LOCATION "${ARIADNE_RENDERING_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES
                "${ARIADNE_RENDERING_INCLUDE_PARENT_DIR};${ARIADNE_RENDERING_INCLUDE_DIR}"
        )
    endif()
endif()
