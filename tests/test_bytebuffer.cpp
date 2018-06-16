#include <gtest/gtest.h>
#include <libreactor/ByteBuffer.h>

using namespace reactor;

TEST(consstruct, test_bytebuffer)
{

    ByteBuffer buffer(4);
    ASSERT_TRUE(buffer.capacity() == 4);

    ASSERT_TRUE(buffer.has_remaining());
    ASSERT_TRUE(buffer.remaining() == 4);


    int a = 7;
    buffer.put(&a, sizeof(a));

    ASSERT_FALSE(buffer.has_remaining());
    ASSERT_TRUE(buffer.remaining() == 0);

    int b;
    buffer.flip();
    buffer.get(&b, sizeof(b));

    ASSERT_TRUE(a == b);


    ASSERT_TRUE(buffer.position() == 4);
    buffer.rewind();
    ASSERT_TRUE(buffer.remaining() == 4);
    ASSERT_TRUE(buffer.position() == 0);
}


TEST(test_reset, test_bytebuffer)
{
    ByteBuffer buffer(10);
    ASSERT_TRUE(buffer.capacity() == 10);

    int a = 9;
    buffer.put(&a, sizeof(a));

    buffer.mark();

    ASSERT_TRUE(buffer.remaining() == 6);

    int c;
    int b = 8;
    buffer.put(&b, sizeof(b));

    ASSERT_TRUE(buffer.remaining() == 2);

    buffer.reset();
    ASSERT_TRUE(buffer.remaining() == 6);

    buffer.get(&c, sizeof(c));
    ASSERT_TRUE(b == c);
    ASSERT_TRUE(buffer.remaining() == 2);
}


int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
