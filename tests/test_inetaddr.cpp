
#include <gtest/gtest.h>
#include <InetAddress.h>

using namespace net;

TEST(test_addr, test_addr)
{
  InetAddress addr4 = InetAddress::get_by_address("127.0.0.1", InetAddress::INET);
  ASSERT_TRUE(addr4.v4());
  InetAddress addr6 = InetAddress::get_by_address("0:0:0:0:0:FFFF:204.152.189.116", InetAddress::INET6);
  ASSERT_TRUE(addr6.v6());

  ASSERT_ANY_THROW(InetAddress::get_by_address("1279.0.0.1", InetAddress::INET));

  InetAddress addr = InetAddress::any();
  addr = InetAddress::any(InetAddress::INET6);
}


int main(int argc, char* argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}