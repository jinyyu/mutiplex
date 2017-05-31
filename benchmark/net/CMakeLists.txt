set(LIBS
        ${LIBS} libnet)

add_executable(echo_server echo/server.cpp)
target_link_libraries(echo_server ${LIBS} )

add_executable(datetime_server datetime/server.cpp)
target_link_libraries(datetime_server ${LIBS} )

add_executable(datetime_client datetime/client.cpp)
target_link_libraries(datetime_client ${LIBS} )

add_executable(pingpong_server pingpong/server.cpp)
target_link_libraries(pingpong_server ${LIBS} )


add_executable(pingpong_client pingpong/client.cpp)
target_link_libraries(pingpong_client ${LIBS} )