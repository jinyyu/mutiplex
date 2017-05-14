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

  LOG("%s",status.to_string().c_str() );



  status = Status::io_error("io error");
  ASSERT_TRUE(status.is_io_error());

  LOG("%s",status.to_string().c_str() );

  status = Status::io_error(nullptr);

  LOG("%s",status.to_string().c_str() );

}


int main(int argc, char* argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}