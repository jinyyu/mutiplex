#include <gtest/gtest.h>
#include <libreactor/Timestamp.h>
#include <unistd.h>

using namespace reactor;

TEST(test_timestamp, test_timestamp)
{
    Timestamp t1 = Timestamp::currentTime();
    Timestamp t2 = t1;
    sleep(1);
    Timestamp t3 = Timestamp::currentTime();

    ASSERT_TRUE(t1 == t2);
    ASSERT_TRUE(t1 < t3);
    ASSERT_TRUE(t1 <= t3);
    ASSERT_TRUE(t3 > t1);
    ASSERT_TRUE(t3 >= t1);
    ASSERT_TRUE(t1 == t1);
    ASSERT_TRUE(t1 != t3);

    t3 = t2 = t1;

    ASSERT_TRUE(t3 == t1);

    printf("%s\n", t1.to_string().c_str());

}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

