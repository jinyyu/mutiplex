#include <InetSocketAddress.h>
#include <gtest/gtest.h>
#include <InetAddress.h>
#include <Status.h>
#include <Logger.h>


using namespace net;

TEST(test, test)
{
  InetAddress in = InetAddress::get_by_address("10.12.18.19");
  InetSocketAddress socketaddr(in, 80);

  InetAddress out = socketaddr.get_address();
  ASSERT_TRUE(in == out);


  Status status;
  in = InetAddress::get_by_host("www.baidu.com", status);
  if (status.is_ok()) {
    LOG("%s", in.to_string().c_str());
    InetSocketAddress addr(in, 10);
    LOG("%s", addr.get_address().to_string().c_str());
  }



  InetSocketAddress add("wwww.baidu.com", 80);
  out = add.get_address();
  LOG("%s", out.to_string().c_str());
  
}



int main(int argc, char* argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

