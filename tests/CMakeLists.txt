macro(add_om_test _TEST_NAME)

    add_executable(${_TEST_NAME}_tests
            ${_TEST_NAME}_tests.cpp
            ../src/utils.cpp)

    target_link_libraries(${_TEST_NAME}_tests
            gtest gtest_main
            gmock gmock_main
            ${LIBRARIES})

    target_include_monero_directories(${_TEST_NAME}_tests)

    target_include_directories(${_TEST_NAME}_tests
        PRIVATE ../src/xmregcore)

    add_test(NAME ${_TEST_NAME}_tests COMMAND ${_TEST_NAME}_tests)

endmacro()

resource_dir("res")

add_om_test(utils)

