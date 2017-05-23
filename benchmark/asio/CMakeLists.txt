set(LIBS
        ${LIBS} libnet)


add_executable(asio_echo_server echo/server.cpp)
target_link_libraries(asio_echo_server ${LIBS})


add_executable(asio_datetime_server datetime/server.cpp)
target_link_libraries(asio_datetime_server ${LIBS})