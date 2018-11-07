
set(LIBS
        ${LIBS} mutiplex)

add_executable(test_InetAddress InetAddress.cpp)
target_link_libraries(test_InetAddress ${LIBS})

add_executable(test_ByteBuffer ByteBuffer.cpp)
target_link_libraries(test_ByteBuffer ${LIBS})

add_executable(test_CircularBuffer CircularBuffer.cpp)
target_link_libraries(test_CircularBuffer ${LIBS})

