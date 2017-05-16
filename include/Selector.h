#ifndef LIBNET_DISTRIBUTION_SELECTOR_H
#define LIBNET_DISTRIBUTION_SELECTOR_H


namespace net
{

class Selector
{
public:
  Selector();
  ~Selector();

private:
  int epoll_fd_;

};




}

#endif //LIBNET_DISTRIBUTION_SELECTOR_H
