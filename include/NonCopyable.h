#ifndef LIBNET_DISTRIBUTION_NONCOPYABLE_H
#define LIBNET_DISTRIBUTION_NONCOPYABLE_H


namespace net {

class NonCopyable
{
 public:

  NonCopyable(const NonCopyable &instance) = delete;

  NonCopyable &operator=(const NonCopyable &) = delete;

 protected:
  NonCopyable() = default;
  ~NonCopyable() = default;
};

}

#endif //LIBNET_DISTRIBUTION_NONCOPYABLE_H
