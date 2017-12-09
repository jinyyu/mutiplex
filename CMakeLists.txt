cmake_minimum_required(VERSION 3.5)
project(net4cxx-distribution)

set(CMAKE_CXX_STANDARD 11)

set(NET4CXX_VERSION "0.1")
set(NET4CXX_BUILD   "1")

include_directories(${CMAKE_SOURCE_DIR})

find_package(PkgConfig REQUIRED)
pkg_check_modules(log4cxx liblog4cxx REQUIRED)
if (log4cxx_FOUND)
    include_directories(${log4cxx_INCLUDE_DIRS})
    set(LIBS
            ${LIBS} ${log4cxx_LIBRARIES})
endif (log4cxx_FOUND)

set(net4cxx_HEARERS
        net4cxx/common/utils.h
        net4cxx/common/InetAddress.h
        net4cxx/common/InetSocketAddress.h
        net4cxx/common/Timestamp.h
        net4cxx/common/Slice.h
        net4cxx/common/Status.h
        net4cxx/common/ByteBuffer.h
        net4cxx/common/callbacks.h
        net4cxx/common/SingalHandler.h
        net4cxx/reactor/Acceptor.h
        net4cxx/reactor/Channel.h
        net4cxx/common/CircularBuffer.h
        net4cxx/reactor/Connection.h
        net4cxx/reactor/Context.h
        net4cxx/reactor/EventLoop.h
        net4cxx/reactor/SelectionKey.h
        net4cxx/reactor/ServerSocket.h
        net4cxx/reactor/Selector.h
        net4cxx/reactor/Session.h
        net4cxx/reactor/TcpServer.h
        net4cxx/reactor/Timer.h)

set(net4cxx_SRCS
        src/common/utils.cpp
        src/common/Status.cpp
        src/common/ByteBuffer.cpp
        src/common/Timestamp.cpp
        src/common/InetAddress.cpp
        src/common/InetSocketAddress.cpp
        src/common/CircularBuffer.cpp
        src/common/SingalHandler.cpp
        src/reactor/SelectionKey.cpp
        src/reactor/Selector.cpp
        src/reactor/Channel.cpp
        src/reactor/EventLoop.cpp
        src/reactor/ServerSocket.cpp
        src/reactor/Acceptor.cpp
        src/reactor/TcpServer.cpp
        src/reactor/Connection.cpp
        src/reactor/Session.cpp
        src/reactor/Timer.cpp)

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
configure_file(net4cxx.spec.in net4cxx.spec)
configure_file(log4cxx.conf.in log4cxx.conf)

install(TARGETS  net4cxx
        ARCHIVE DESTINATION lib)

install(DIRECTORY net4cxx
        DESTINATION include)

