cmake_minimum_required(VERSION 3.5)
project(evcpp-distribution)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR})

find_package(PkgConfig REQUIRED)

set(HEARERS
    evcpp/InetAddress.h
    evcpp/InetSocketAddress.h
    evcpp/Timestamp.h
    evcpp/Slice.h
    evcpp/Status.h
    evcpp/ByteBuffer.h
    evcpp/callbacks.h
    evcpp/Acceptor.h
    evcpp/CircularBuffer.h
    evcpp/Connection.h
    evcpp/EventLoop.h
    evcpp/ServerSocket.h
    evcpp/Session.h
    evcpp/TcpServer.h
    )

set(SRCS
    src/EventSource.cpp
    src/Status.cpp
    src/ByteBuffer.cpp
    src/Timestamp.cpp
    src/InetAddress.cpp
    src/InetSocketAddress.cpp
    src/CircularBuffer.cpp
    src/EventLoop.cpp
    src/ServerSocket.cpp
    src/Acceptor.cpp
    src/TcpServer.cpp
    src/Connection.cpp
    src/Session.cpp
    )

add_library(evcpp ${SRCS} ${HEARERS})
target_link_libraries(evcpp ${LIBS})

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

add_subdirectory(benchmark/evcpp)


install(TARGETS evcpp
        ARCHIVE DESTINATION lib)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/evcpp
        DESTINATION include)

