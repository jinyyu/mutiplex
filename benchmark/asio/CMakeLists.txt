

add_executable(asio_echo_server echo/server.cpp)
target_link_libraries(asio_echo_server ${LIBS})