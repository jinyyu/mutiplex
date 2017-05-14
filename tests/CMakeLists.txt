

add_executable(test_status test_status.cpp)
target_link_libraries(test_status ${LIBS} gtest libnet)

add_executable(test_log test_log.cpp)
target_link_libraries(test_log libnet)