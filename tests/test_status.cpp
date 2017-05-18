#include <gtest/gtest.h>
#include <Status.h>
#include <Logger.h>


using namespace net;

TEST(test_status, test_status)
{
  Status status;
  status.to_string();
  ASSERT_TRUE(status.is_ok());

  const char* msg = "test msg";
  status = Status::invalid_argument(msg);
  ASSERT_TRUE(status.is_invalid_argument());

  LOG_INFO("%s",status.to_string().c_str() );



  status = Status::io_error("io error");
  ASSERT_TRUE(status.is_io_error());

  LOG_INFO("%s",status.to_string().c_str() );

  status = Status::io_error(nullptr);

  LOG_INFO("%s",status.to_string().c_str() );

  Status st1;
  Status st2;

  Status st3 = Status::invalid_argument("aaa");

  st1 = st2 = st3;
  ASSERT_TRUE(st1.is_invalid_argument());
  ASSERT_TRUE(st2.is_invalid_argument());

}


int main(int argc, char* argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}