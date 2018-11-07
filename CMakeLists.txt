cmake_minimum_required(VERSION 3.5)
project(mutiplex-distribution)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR})

find_package(PkgConfig REQUIRED)

set(HEARERS
    mutiplex/InetAddress.h
    mutiplex/Timestamp.h
    mutiplex/Slice.h
    mutiplex/Status.h
    mutiplex/ByteBuffer.h
    mutiplex/callbacks.h
    mutiplex/Acceptor.h
    mutiplex/CircularBuffer.h
    mutiplex/Connection.h
    mutiplex/EventLoop.h
    mutiplex/ServerSocket.h
    mutiplex/Session.h
    mutiplex/TcpServer.h
    )

set(SRCS
    src/EventSource.cpp
    src/Status.cpp
    src/ByteBuffer.cpp
    src/Timestamp.cpp
    src/InetAddress.cpp
    src/CircularBuffer.cpp
    src/EventLoop.cpp
    src/ServerSocket.cpp
    src/Acceptor.cpp
    src/TcpServer.cpp
    src/Connection.cpp
    src/Session.cpp
    )

add_library(mutiplex ${SRCS} ${HEARERS})
target_link_libraries(mutiplex ${LIBS})

pkg_check_modules(GoogleTest gtest gtest_main)
if (GoogleTest_FOUND)
    include_directories(${GoogleTest_INCLUDE_DIRS})
    set(LIBS
        ${LIBS} ${GoogleTest_LIBRARIES} pthread)
    add_subdirectory(tests)
endif (GoogleTest_FOUND)


find_package(Boost COMPONENTS system thread)
if (Boost_FOUND)
    set(LIBS
        ${LIBS} Boost::system Boost::thread)
    add_subdirectory(benchmark/asio)
endif (Boost_FOUND)

add_subdirectory(benchmark/mutiplex)


install(TARGETS mutiplex
        ARCHIVE DESTINATION lib)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/evcpp
        DESTINATION include)

