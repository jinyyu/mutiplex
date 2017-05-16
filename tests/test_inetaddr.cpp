
#include <gtest/gtest.h>
#include <InetAddress.h>
#include <Status.h>
#include <Logger.h>

using namespace net;

TEST(test_addr, test_addr)
{
  InetAddress addr4 = InetAddress::get_by_address("127.0.0.1", InetAddress::INET);
  LOG("ip = %s", addr4.to_string().c_str());
  ASSERT_TRUE(addr4.v4());
  InetAddress addr6 = InetAddress::get_by_address("0:0:0:0:0:FFFF:204.152.189.116", InetAddress::INET6);
  ASSERT_TRUE(addr6.v6());
  LOG("ip = %s", addr6.to_string().c_str());

  ASSERT_ANY_THROW(InetAddress::get_by_address("1279.0.0.1", InetAddress::INET));

  InetAddress addr = InetAddress::any();
  addr = InetAddress::any(InetAddress::INET6);
}

TEST(test_resolve, test_addr)
{

  Status status = Status::invalid_argument("aaa");
  InetAddress a = InetAddress::get_by_host("www.baidu.com", status);
  ASSERT_TRUE(status.is_ok());
  LOG("ip = %s", a.to_string().c_str());

  InetAddress::get_by_host("localhost", status);
  ASSERT_TRUE(status.is_ok());

  InetAddress::get_by_host("www.baidu.comppp", status);
  ASSERT_TRUE(!status.is_ok());

}




int main(int argc, char* argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}