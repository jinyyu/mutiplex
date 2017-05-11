set(SRC
        test_status.hpp
        main.cpp
        )




add_executable(test_main ${SRC})
target_link_libraries(test_main ${LIBS} gtest pthread libnet)