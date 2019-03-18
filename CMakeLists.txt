cmake_minimum_required(VERSION 3.8)

set(PROJECT_NAME
        jsonifyxmrtx)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

list(APPEND CMAKE_MODULE_PATH 
    "${CMAKE_CURRENT_LIST_DIR}/src/xmregcore/cmake"
    "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(MyUtils)


# add XMREGCORE submodule
set(BUILD_XMREGCORE_TESTS OFF CACHE INTERNAL "")
add_subdirectory(src/xmregcore)

set(SOURCE_FILES
        main.cpp
        src/utils.cpp
        src/foundobjectprocessor.cpp)

add_executable(${PROJECT_NAME}
        ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
    PRIVATE XMREG::core)

if (${BUILD_TESTS})
    include_directories(
        ${CMAKE_SOURCE_DIR}/ext/googletest/googletest/include
        ${CMAKE_SOURCE_DIR}/ext/googletest/googlemock/include)
endif()

if (${BUILD_TESTS})
    enable_testing()
    add_subdirectory(ext/googletest)
    add_subdirectory(tests)
endif()
