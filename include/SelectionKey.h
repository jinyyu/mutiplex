#ifndef LIBNET_DISTRIBUTION_SELECTIONKEY_H
#define LIBNET_DISTRIBUTION_SELECTIONKEY_H

namespace net
{

class Channel;
class Selector;

class SelectionKey
{

public:
  static const int OP_IN;
  static const int OP_OUT;
  static const int OP_PRI;
  static const int OP_ERR;
  static const int OP_HUP;

  explicit SelectionKey(Channel* channel) : channel_(channel), interest_ops_(0), ready_ops_(0) {}

  //Requests that the registration of this key's channel with its selector be cancelled
  void cancel();

  //Returns the channel for which this key was created
  Channel* channel() const { return channel_; }

  //Retrieves this key's interest set
  int interest_ops() const { return interest_ops_; }

  //Sets this key's interest set to the given value
  void interest_ops(int ops) { interest_ops_ = ops; }

  //Tests whether this key's channel is ready for reading
  bool is_readable() const { return ready_ops_ & OP_IN; }

  //Tests whether this key's channel is ready for writing
  bool is_writable() const { return  ready_ops_ & OP_OUT; }

  //Retrieves this key's ready-operation set
  int ready_ops() const { return ready_ops_; }

  //Sets this key's ready set to the given value
  void ready_ops(int ops) { ready_ops_ = ops; }

  Selector* selector() const;


private:
  friend class Selector;
  friend class Channel;

  void enable_ops(int op) { interest_ops_ |= op; }

  void disable_ops(int op) { interest_ops_ &= ~op; }

  int fd();

private:
  Channel* channel_;
  Selector* selector_;
  int interest_ops_;
  int ready_ops_;

};

}

#endif //LIBNET_DISTRIBUTION_SELECTIONKEY_H
