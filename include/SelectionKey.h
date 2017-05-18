#ifndef LIBNET_DISTRIBUTION_SELECTIONKEY_H
#define LIBNET_DISTRIBUTION_SELECTIONKEY_H
#include <inttypes.h>

namespace net
{

class Channel;
class Selector;

class SelectionKey
{

public:
  static const uint32_t OP_IN;
  static const uint32_t OP_OUT;
  static const uint32_t OP_PRI;
  static const uint32_t OP_ERR;
  static const uint32_t OP_HUP;

  explicit SelectionKey(Channel* channel) : channel_(channel), interest_ops_(0), ready_ops_(0) {}

  //Requests that the registration of this key's channel with its selector be cancelled
  void cancel();

  //Returns the channel for which this key was created
  Channel* channel() const { return channel_; }

  //Retrieves this key's interest set
  uint32_t interest_ops() const { return interest_ops_; }

  //Sets this key's interest set to the given value
  void interest_ops(uint32_t ops) { interest_ops_ = ops; }

  //Tests whether this key's channel is ready for reading
  bool is_readable() const { return ready_ops_ & OP_IN; }

  //Tests whether this key's channel is ready for writing
  bool is_writable() const { return  ready_ops_ & OP_OUT; }

  //Retrieves this key's ready-operation set
  uint32_t ready_ops() const { return ready_ops_; }

  //Sets this key's ready set to the given value
  void ready_ops(uint32_t ops) { ready_ops_ = ops; }

  Selector* selector() const;


private:
  friend class Selector;
  friend class Channel;

  void enable_ops(uint32_t op) { interest_ops_ |= op; }

  void disable_ops(uint32_t op) { interest_ops_ &= ~op; }

  int fd();

private:
  Channel* channel_;
  Selector* selector_;
  uint32_t interest_ops_;
  uint32_t ready_ops_;

};

}

#endif //LIBNET_DISTRIBUTION_SELECTIONKEY_H
