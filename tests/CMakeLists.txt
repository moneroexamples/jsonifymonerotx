macro(add_om_test _TEST_NAME)

    add_executable(${_TEST_NAME}_tests
            ${_TEST_NAME}_tests.cpp
            ../src/xmregcore/src/tools.cpp
            ../src/utils.cpp
            ../src/foundobjectprocessor.cpp
            ../src/xmregcore/tests/JsonTx.cpp)

    target_link_libraries(${_TEST_NAME}_tests
            gtest gtest_main
            gmock gmock_main
            XMREG::core)

    add_test(NAME ${_TEST_NAME}_tests COMMAND ${_TEST_NAME}_tests)

endmacro()

resource_dir("res")

add_om_test(utils)
add_om_test(foundobjectprocessor)



######################################
########## CODE COVERAGE #############
######################################

include(CodeCoverage)

APPEND_COVERAGE_COMPILER_FLAGS()

set(COVERAGE_LCOV_EXCLUDES 
    '../ext/*' '*boost/*' '*c++/*'
    '*monero/*' '*googletest/*' '*googlemock/*')

#SETUP_TARGET_FOR_COVERAGE_GCOVR_HTML(
#    NAME utils_cov
#    EXECUTABLE utils_tests)

SETUP_TARGET_FOR_COVERAGE_LCOV(
    NAME coverage
    EXECUTABLE utils_tests 
               foundobjectprocessor_tests)

