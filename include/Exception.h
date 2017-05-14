#ifndef LIBNET_DISTRIBUTION_EXCEPTION_H
#define LIBNET_DISTRIBUTION_EXCEPTION_H
#include "Status.h"

#include <exception>

namespace net {

class Exception : public std::exception
{
 public:

  static Exception not_found(const char* msg) { return Exception(Status::not_found(msg)); }

  static Exception not_supported(const char* msg) { return Exception(Status::not_supported(msg)); }

  static Exception invalid_argument(const char* msg) { return Exception(Status::invalid_argument(msg)); }

  static Exception io_error(const char* msg) { return Exception(Status::io_error(msg)); }

  Status status() { return status_; }

 private:
  Exception(const Status& status) : status_(status) {}

 private:
  Status status_;
};

}

#endif //LIBNET_DISTRIBUTION_EXCEPTION_H
