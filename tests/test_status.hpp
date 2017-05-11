#include <gtest/gtest.h>
#include <Status.h>


using namespace net;

TEST(test_status, test_status)
{
  Status status;
  status.to_string();
  ASSERT_TRUE(status.is_ok());

  const char* msg = "test msg";
  status = Status::invalid_argument(msg);
  ASSERT_TRUE(status.is_invalid_argument());
  status.to_string();


  status = Status::io_error("io error");
  ASSERT_TRUE(status.is_io_error());
}