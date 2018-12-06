cmake_minimum_required(VERSION 3.2)

set(PROJECT_NAME
        jsonifyxmrtx)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)

if (NOT MONERO_DIR)
    set(MONERO_DIR ~/monero)
endif()

option(BUILD_TESTS "Build tests for the project" OFF)

message(STATUS MONERO_DIR ": ${MONERO_DIR}")

set(MONERO_SOURCE_DIR ${MONERO_DIR}
        CACHE PATH "Path to the root directory for Monero")

# set location of monero build tree
set(MONERO_BUILD_DIR ${MONERO_SOURCE_DIR}/build/release/
        CACHE PATH "Path to the build directory for Monero")

set(MY_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake"
        CACHE PATH "The path to the cmake directory of the current project")

list(APPEND CMAKE_MODULE_PATH "${MY_CMAKE_DIR}")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/src/xmregcore/cmake")

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${MONERO_BUILD_DIR}"
        CACHE PATH "Add Monero directory for library searching")

include(MyUtils)

# find boost
find_package(Boost COMPONENTS
        system
        filesystem
        thread
        date_time
        chrono
        regex
        serialization
        program_options
        date_time
        REQUIRED)


# add XMREGCORE submodule
set(BUILD_XMREGCORE_TESTS OFF CACHE INTERNAL "")
add_subdirectory(src/xmregcore)

# add ext/ subfolder
#add_subdirectory(ext/)


set(SOURCE_FILES
        main.cpp)

add_executable(${PROJECT_NAME}
        ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
    PRIVATE src/xmregcore)

# include monero headers
target_include_monero_directories(${PROJECT_NAME})

message("Monero_LIBRARIES: ${Monero_LIBRARIES}" )

set(LIBRARIES
        myxrmcore
        ${Monero_LIBRARIES}
        ${Boost_LIBRARIES}
        sodium
        pthread
        unbound
        curl
        ssl
        crypto)

if(APPLE)
    set(LIBRARIES ${LIBRARIES} "-framework IOKit -framework Foundation")
else()
    set(LIBRARIES ${LIBRARIES} atomic)
endif()

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND NOT WIN32)
    set(LIBRARIES ${LIBRARIES} unwind)
endif()


if (WIN32)
    set(LIBRARIES ${LIBRARIES}
            wsock32
            ntdll
            ws2_32
            Iphlpapi)
else()
    set(LIBRARIES ${LIBRARIES} dl)
endif()

find_package(HIDAPI)
set(LIBRARIES ${LIBRARIES} ${HIDAPI_LIBRARIES})

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${LIBRARIES})


#if (${BUILD_TESTS})
#    include_directories(
#        ${CMAKE_SOURCE_DIR}/ext/googletest/googletest/include
#        ${CMAKE_SOURCE_DIR}/ext/googletest/googlemock/include)
#endif()

#if (${BUILD_TESTS})
#    enable_testing()
#    add_subdirectory(ext/googletest)
#    add_subdirectory(tests)
#endif()
