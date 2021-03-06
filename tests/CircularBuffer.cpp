#include <gtest/gtest.h>
#include <mutiplex/CircularBuffer.h>
#include <string>

using namespace muti;


TEST(test_cirularbuffer, test_cirularbuffer)
{
    CircularBuffer buffer(30);
    uint32_t size = buffer.capacity();
    ASSERT_TRUE(size == 32);

    char buf_src[100];
    char buf_out[100];

    for (int j = 0; j < 10000; ++j) {

        for (size_t i = 0; i < sizeof(buf_src); ++i) {
            buf_src[i] = static_cast<char>((i * 10 - 9) ^ (i - 1));
        }

        buffer.put(buf_src, 40);

        buffer.put(buf_src + 40, 60);

        buffer.get(buf_out, 60);

        ASSERT_TRUE(memcmp(buf_src, buf_out, 60) == 0);
        buffer.get(buf_out + 40, 40);
        ASSERT_TRUE(memcmp(buf_src + 60, buf_out + 40, 40) == 0);
    }
}

TEST(resize_0, test_circularbuffer)
{
    char buf_src[100];
    char buf_dest[100];
    for (int i = 0; i < 100; ++i) {
        buf_src[i] = static_cast<char>((i * 10 - 9) ^ (i - 1));
    }
    CircularBuffer buffer(2);

    buffer.put(buf_src, 100);
    buffer.get(buf_dest, 100);

    buffer.put(buf_src, 100);
    buffer.get(buf_dest, 100);

    ASSERT_TRUE(memcmp(buf_src, buf_dest, 100) == 0);
}

TEST(resize_1, test_circularbuffer)
{
    char buf_src[128];
    char buf_dest[128];
    for (int i = 0; i < 128; ++i) {
        buf_src[i] = static_cast<char>((i * 10 - 9) ^ (i - 1));
    }
    CircularBuffer buffer(128);

    buffer.put(buf_src, 128);
    ASSERT_TRUE(buffer.full());
    buffer.get(buf_dest, 64);
    ASSERT_TRUE(!buffer.full());
    buffer.get(buf_dest + 64, 64);
    ASSERT_TRUE(memcmp(buf_src, buf_dest, 128) == 0);
}

TEST(resize_2, test_circularbuffer)
{
    char buf_src[26];
    for (int i = 0; i < 26; ++i) {
        buf_src[i] = static_cast<char>('a' + ((i*8) & i));
    }

    CircularBuffer buffer(1);

    for (int i = 0; i < 100; ++i) {
        int c = buf_src[i];
        buffer.put(&c, sizeof(char));
    }

    char buf_dst[26];

    uint32_t ret = buffer.get(buf_dst, 26);
    ASSERT_TRUE(ret = 26);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

