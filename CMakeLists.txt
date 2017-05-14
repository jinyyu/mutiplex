cmake_minimum_required(VERSION 3.5)
project(libnet-distribution)

set(CMAKE_CXX_STANDARD 11)


set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/modules/")

include_directories(${PROJECT_SOURCE_DIR}/include)

set(libnet_SRCS
        src/Status.cpp
        src/Logger.cpp
        src/ByteBuffer.cpp include/Exception.h)

add_library(libnet ${libnet_SRCS})
target_link_libraries(libnet ${LIBS})


find_package(LibGtest QUIET)
if (LibGtest_FOUND)
    include_directories(${LibGtest_INCLUDE_DIR})
    set(LIBS
            ${LIBS} ${LibGtest_LIBRARIE} gtest pthread)
    add_subdirectory(tests)
endif (LibGtest_FOUND)



