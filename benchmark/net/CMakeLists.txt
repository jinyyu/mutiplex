set(LIBS
        ${LIBS} libnet)

add_executable(echo_server echo/server.cpp)
target_link_libraries(echo_server ${LIBS} )