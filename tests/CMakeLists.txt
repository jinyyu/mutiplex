
set(LIBS
        ${LIBS} gtest libnet)


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
