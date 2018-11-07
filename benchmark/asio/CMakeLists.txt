set(LIBS
        ${LIBS} mutiplex)


add_executable(asio_echo_server echo/server.cpp)
target_link_libraries(asio_echo_server ${LIBS})

add_executable(asio_datetime_server datetime/server.cpp)
target_link_libraries(asio_datetime_server ${LIBS})

add_executable(asio_pingpong_server pingpong/handler_allocator.hpp pingpong/server.cpp)
target_link_libraries(asio_pingpong_server ${LIBS})

add_executable(asio_pingpong_client pingpong/handler_allocator.hpp pingpong/client.cpp)
target_link_libraries(asio_pingpong_client ${LIBS})