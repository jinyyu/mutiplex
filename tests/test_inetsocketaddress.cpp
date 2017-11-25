#include <net4cxx/InetSocketAddress.h>
#include <gtest/gtest.h>
#include <net4cxx/InetAddress.h>
#include <net4cxx/Status.h>

using namespace net4cxx;

TEST(test, test)
{
    Status status;
    InetAddress in = InetAddress::get_by_address("10.12.18.19", AF_INET, status);
    InetSocketAddress socketaddr(in, 80);
    ASSERT_TRUE(status.is_ok());

    InetAddress out = socketaddr.get_address();
    ASSERT_TRUE(in == out);


    InetSocketAddress add("wwww.baidu.com", 80);
    out = add.get_address();
    printf("%s\n", out.to_string().c_str());

    ASSERT_TRUE(add.port() == 80);

    in = InetAddress::get_by_host("www.baidu.com", status);
    if (status.is_ok()) {
        printf("%s\n", in.to_string().c_str());
        InetSocketAddress addr(in, 10);
        printf("%s\n", addr.get_address().to_string().c_str());
    }

}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

