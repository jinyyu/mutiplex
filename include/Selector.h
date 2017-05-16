#ifndef LIBNET_DISTRIBUTION_SELECTOR_H
#define LIBNET_DISTRIBUTION_SELECTOR_H
#include "NonCopyable.h"
#include "SelectionKey.h"

namespace net
{

class Selector : NonCopyable
{
public:
  Selector();

  ~Selector();

  void add(const SelectionKey& selection_key);

  void modify(const SelectionKey& selection_key);

  void remove(const SelectionKey& selection_key);

private:
  void control(int op, const SelectionKey& selection_key);

  int epoll_fd_;

};




}

#endif //LIBNET_DISTRIBUTION_SELECTOR_H
