find_package(Qt5 COMPONENTS
        Test
        REQUIRED
)

include_directories(../src)

function(generate_test test_path)
        get_filename_component(test_name ${test_path} NAME_WE)
        add_executable(${test_name}
                ${test_path}
        )
        target_link_libraries(${test_name}
                ${PROJECT_NAME}_lib
                Qt5::Test
        )
        add_test(NAME ${test_name} COMMAND ${test_name}
)
endfunction()

enable_testing()
generate_test(utils/vector2d_test.cpp)
generate_test(serialization/model_serialization_test.cpp)
