include(CTest)

set(UNIT_TESTS
    test_figure
    test_null_rendering
    test_rendering_utility_api
    test_default_rendering_regression
    test_default_rendering_robustness
)

foreach(TEST ${UNIT_TESTS})
    add_executable(${TEST} ${TEST}.cpp)
    target_link_libraries(${TEST} PRIVATE ariadne-rendering rendering)
    if("${TEST}" STREQUAL "test_default_rendering_regression")
        target_compile_definitions(${TEST} PRIVATE
            ARIADNE_RENDERING_GOLDEN_DIR="${CMAKE_CURRENT_SOURCE_DIR}/golden")
    endif()
    add_test(NAME ${TEST} COMMAND $<TARGET_FILE:${TEST}>)
    if(WIN32)
        set_tests_properties(${TEST} PROPERTIES
            ENVIRONMENT_MODIFICATION
                "PATH=path_list_prepend:$<TARGET_FILE_DIR:ariadne-rendering>"
        )
    endif()
endforeach()

set_tests_properties(test_default_rendering_regression test_default_rendering_robustness PROPERTIES
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

add_custom_target(tests)
add_dependencies(tests ${UNIT_TESTS})
