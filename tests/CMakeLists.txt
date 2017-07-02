
set(LIBS
        ${LIBS} libnet)


add_executable(Tx test.cpp)
target_link_libraries(Tx ${LIBS} )

add_executable(test_status test_status.cpp)
target_link_libraries(test_status ${LIBS} )

add_executable(test_log test_log.cpp)
target_link_libraries(test_log ${LIBS})

add_executable(test_bytebuffer test_bytebuffer.cpp)
target_link_libraries(test_bytebuffer ${LIBS} )

add_executable(test_timestamp test_timestamp.cpp)
target_link_libraries(test_timestamp ${LIBS} )

add_executable(test_inetaddr test_inetaddr.cpp)
target_link_libraries(test_inetaddr ${LIBS} )

add_executable(test_inetsocketaddr test_inetsocketaddress.cpp)
target_link_libraries(test_inetsocketaddr ${LIBS} )

add_executable(test_eventloop test_eventloop.cpp)
target_link_libraries(test_eventloop ${LIBS} )

add_executable(test_acceptor test_acceptor.cpp)
target_link_libraries(test_acceptor ${LIBS} )

add_executable(test_tcpserver test_tcpserver.cpp)
target_link_libraries(test_tcpserver ${LIBS} )

add_executable(client_shutdown client_shutdown.cpp)
target_link_libraries(client_shutdown ${LIBS} )

add_executable(test_util test_util.cpp)
target_link_libraries(test_util ${LIBS} )

add_executable(test_circularbuffer test_circularbuffer.cpp)
target_link_libraries(test_circularbuffer ${LIBS} )

add_executable(test_timer test_timer.cpp)
target_link_libraries(test_timer ${LIBS} )