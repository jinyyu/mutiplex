#include <gtest/gtest.h>
#include <mutiplex/InetAddress.h>

using namespace muti;

TEST(test_addr, test_addr)
{
    InetAddress addr1("127.0.0.1:443");
    ASSERT_TRUE(addr1.to_string() == "127.0.0.1:443");
    ASSERT_TRUE(addr1.ip_str() == "127.0.0.1");
    ASSERT_TRUE(addr1.host_port() == 443);
}

TEST(test_resolve, test_addr)
{
    InetAddress addr;
    bool ok = InetAddress::resolve("wwww.baidu.com", "http", addr);
    ASSERT_TRUE(ok);
    ASSERT_TRUE(addr.host_port() == 80);

    fprintf(stderr, "ip = %s, port = %d\n", addr.ip_str().c_str(), addr.host_port());
    fprintf(stderr, "str = %s\n", addr.to_string().c_str());

    ASSERT_FALSE(InetAddress::resolve("wwww.google.com", "http", addr));

}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}