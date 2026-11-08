# The Benchmarking module registers this directory and owns the aggregate
# benchmarks target. Rendering's compiled objects already include Utility;
# add only Threading/Logging objects not present in Rendering's bundle.
get_target_property(_rendering_objects rendering ARIADNE_PROJECT_OBJECT_TARGETS)
get_target_property(_threading_objects threading ARIADNE_PROJECT_OBJECT_TARGETS)
if(NOT _rendering_objects OR NOT _threading_objects)
    message(FATAL_ERROR "Missing Rendering/Threading object target registrations")
endif()
foreach(_object IN LISTS _rendering_objects)
    list(REMOVE_ITEM _threading_objects "${_object}")
endforeach()

add_executable(benchmark_rendering EXCLUDE_FROM_ALL benchmark_rendering.cpp)
target_link_libraries(benchmark_rendering PRIVATE
    ariadne-rendering rendering ariadne-benchmarking)
foreach(_object IN LISTS _threading_objects)
    target_sources(benchmark_rendering PRIVATE $<TARGET_OBJECTS:${_object}>)
endforeach()
add_dependencies(benchmarks benchmark_rendering)
