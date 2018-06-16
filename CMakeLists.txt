cmake_minimum_required(VERSION 3.5)
project(libreactor-distribution)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR})

find_package(PkgConfig REQUIRED)
pkg_check_modules(log4cxx liblog4cxx REQUIRED)
if (log4cxx_FOUND)
    include_directories(${log4cxx_INCLUDE_DIRS})
    set(LIBS
        ${LIBS} ${log4cxx_LIBRARIES})
endif (log4cxx_FOUND)

set(reactor_HEARERS
    libreactor/utils.h
    libreactor/InetAddress.h
    libreactor/InetSocketAddress.h
    libreactor/Timestamp.h
    libreactor/Slice.h
    libreactor/Status.h
    libreactor/ByteBuffer.h
    libreactor/callbacks.h
    libreactor/SingalHandler.h
    libreactor/Acceptor.h
    libreactor/Channel.h
    libreactor/CircularBuffer.h
    libreactor/Connection.h
    libreactor/Context.h
    libreactor/EventLoop.h
    libreactor/SelectionKey.h
    libreactor/ServerSocket.h
    libreactor/Selector.h
    libreactor/Session.h
    libreactor/TcpServer.h
    libreactor/Timer.h
    )

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

add_subdirectory(benchmark/reactor)


install(TARGETS reactor
        ARCHIVE DESTINATION lib)

install(DIRECTORY reactor
        DESTINATION include)

