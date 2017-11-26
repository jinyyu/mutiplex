cmake_minimum_required(VERSION 3.5)
project(net4cxx-distribution)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR})

find_package(PkgConfig REQUIRED)
pkg_check_modules(log4cxx liblog4cxx REQUIRED)
if (log4cxx_FOUND)
    include_directories(${log4cxx_INCLUDE_DIRS})
    set(LIBS
            ${LIBS} ${log4cxx_LIBRARIES})
endif (log4cxx_FOUND)

set(net4cxx_HEARERS
        net4cxx/utils.h
        net4cxx/Acceptor.h
        net4cxx/ByteBuffer.h
        net4cxx/callbacks.h
        net4cxx/Channel.h
        net4cxx/CircularBuffer.h
        net4cxx/Connection.h
        net4cxx/Context.h
        net4cxx/EventLoop.h
        net4cxx/InetAddress.h
        net4cxx/InetSocketAddress.h
        net4cxx/NonCopyable.h
        net4cxx/SelectionKey.h
        net4cxx/ServerSocket.h
        net4cxx/Selector.h
        net4cxx/Session.h
        net4cxx/Slice.h
        net4cxx/Status.h
        net4cxx/TcpServer.h
        net4cxx/Timer.h
        net4cxx/Timestamp.h)

set(net4cxx_SRCS
        src/utils.cpp
        src/Status.cpp
        src/ByteBuffer.cpp
        src/Timestamp.cpp
        src/InetAddress.cpp
        src/InetSocketAddress.cpp
        src/SelectionKey.cpp
        src/Selector.cpp
        src/Channel.cpp
        src/EventLoop.cpp
        src/ServerSocket.cpp
        src/Acceptor.cpp
        src/TcpServer.cpp
        src/Connection.cpp
        src/CircularBuffer.cpp
        src/Session.cpp
        src/Timer.cpp)

add_library(net4cxx ${net4cxx_SRCS} ${net4cxx_HEARERS})
target_link_libraries(net4cxx ${LIBS})

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

add_subdirectory(benchmark/net)



