
set(LIBS
        ${LIBS} mutiplex)

add_executable(test_address InetAddress.cpp)
target_link_libraries(test_address ${LIBS})
