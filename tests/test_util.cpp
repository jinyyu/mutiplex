#include <net4cxx/common/utils.h>
#include <gtest/gtest.h>
#include <math.h>


using namespace net4cxx;


TEST(test_pow_of_two, test_pow_of_two)
{
  for(uint32_t i = 0; i < 32; ++i) {
    uint32_t value = std::pow(2, i);
    ASSERT_TRUE(is_pow_of_two(value));
  }

  ASSERT_TRUE(!is_pow_of_two(0));

  ASSERT_TRUE(is_pow_of_two(3) == false);

}




int main(int argc, char* argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
