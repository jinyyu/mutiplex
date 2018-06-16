cmake_minimum_required(VERSION 3.5)
project(libreactor-distribution)

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

set(reactor_HEARERS
        reactor/utils.h
        reactor/InetAddress.h
        reactor/InetSocketAddress.h
        reactor/Timestamp.h
        reactor/Slice.h
        reactor/Status.h
        reactor/ByteBuffer.h
        reactor/callbacks.h
        reactor/SingalHandler.h
        reactor/Acceptor.h
        reactor/Channel.h
        reactor/CircularBuffer.h
        reactor/Connection.h
        reactor/Context.h
        reactor/EventLoop.h
        reactor/SelectionKey.h
        reactor/ServerSocket.h
        reactor/Selector.h
        reactor/Session.h
        reactor/TcpServer.h
        reactor/Timer.h)

set(reactor_SRCS
        src/utils.cpp
        src/Status.cpp
        src/ByteBuffer.cpp
        src/Timestamp.cpp
        src/InetAddress.cpp
        src/InetSocketAddress.cpp
        src/CircularBuffer.cpp
        src/SingalHandler.cpp
        src/SelectionKey.cpp
        src/Selector.cpp
        src/Channel.cpp
        src/EventLoop.cpp
        src/ServerSocket.cpp
        src/Acceptor.cpp
        src/TcpServer.cpp
        src/Connection.cpp
        src/Session.cpp
        src/Timer.cpp)

add_library(reactor ${reactor_SRCS} ${reactor_HEARERS})
target_link_libraries(reactor ${LIBS})

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
configure_file(reactor.spec.in reactor.spec)
configure_file(reactor.conf.in reactor.conf)

install(TARGETS  reactor
        ARCHIVE DESTINATION lib)

install(DIRECTORY reactor
        DESTINATION include)

