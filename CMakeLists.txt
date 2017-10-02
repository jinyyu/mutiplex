cmake_minimum_required(VERSION 3.5)
project(libnet-distribution)

set(CMAKE_CXX_STANDARD 11)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++0x)
endif()

include_directories(include)

set(libnet_SRCS
        src/utils.cpp
        src/Status.cpp
        src/Logger.cpp
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
        src/Timer.cpp
        src/TimingWheel.cpp)

add_library(libnet ${libnet_SRCS})
target_link_libraries(libnet ${LIBS})

find_package(PkgConfig REQUIRED)
pkg_check_modules (GoogleTest gtest gtest_main)
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
endif(Boost_FOUND)

add_subdirectory(benchmark/net)



