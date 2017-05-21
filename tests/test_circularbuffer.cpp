#include <utils.h>
#include <gtest/gtest.h>
#include <CircularBuffer.h>
#include <Logger.h>
#include <string>

using namespace net;

TEST(test_cirularbuffer, test_cirularbuffer)
{
  CircularBuffer buffer(256);
  uint32_t size = buffer.capacity();
  ASSERT_TRUE(is_pow_of_two(size));
  LOG_INFO("size = %d", size)

  char buf_src[100];
  char buf_out[100];

  for(int j = 0; j < 10000; ++j) {

    for (int i = 0; i < sizeof(buf_src); ++i) {
      buf_src[i] = static_cast<char>(i);
    }

    buffer.put(buf_src, 40);

    buffer.put(buf_src + 40, 60);

    buffer.get(buf_out, 60);

    ASSERT_TRUE(memcmp(buf_src, buf_out, 60) == 0);
    buffer.get(buf_out + 40, 40);
    ASSERT_TRUE(memcmp(buf_src + 60, buf_out + 40, 40) == 0);
  }





}



int main(int argc, char* argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
