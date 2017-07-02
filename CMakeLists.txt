cmake_minimum_required(VERSION 3.5)
project(libnet-distribution)

set(CMAKE_CXX_STANDARD 11)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++0x)
endif()

set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/modules/")

include_directories(${PROJECT_SOURCE_DIR}/include)

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
        src/Timer.cpp)

add_library(libnet ${libnet_SRCS})
target_link_libraries(libnet ${LIBS})


find_package(LibGtest QUIET)
if (LibGtest_FOUND)
    message("gtest found")
    message("gtest include : ${LibGtest_INCLUDE_DIR}")
    message("gtest lib : ${LibGtest_LIBRARY}")
    include_directories(${LibGtest_INCLUDE_DIR})
    set(LIBS
            ${LIBS} ${LibGtest_LIBRARY} pthread)
    add_subdirectory(tests)
endif (LibGtest_FOUND)


find_package(Boost COMPONENTS system thread)
if (Boost_FOUND)
    message("boost found")
    message("boost include : ${Boost_INCLUDE_DIRS}")
    message("boost lib : ${Boost_LIBRARY_DIRS}")

    set(LIBS
            ${LIBS} Boost::system Boost::thread)
    add_subdirectory(benchmark/asio)
endif(Boost_FOUND)

add_subdirectory(benchmark/net)



