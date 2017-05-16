#ifndef LIBNET_DISTRIBUTION_SELECTIONKEY_H
#define LIBNET_DISTRIBUTION_SELECTIONKEY_H

namespace net
{

class Channel;

class SelectionKey
{
  enum {
    OP_ACCEPT = 0x001,
    OP_READ   = 0x001,
    OP_WRITE  = 0x040,
  };

public:
  SelectionKey(Channel* channel) : channel_(channel) {}

  //Tests whether this key's channel is ready for reading
  bool is_readable() const { return op()  & OP_READ; }

  //Tests whether this key's channel is ready for writing
  bool is_writable() const { return  op() & OP_WRITE; }

  //Tests whether this key's channel is ready to accept a new socket connection
  bool is_acceptable() const { return op() & OP_ACCEPT; }

  Channel* channel() const { return channel_; }

  int op() const;

private:
  Channel* channel_;

};

}

#endif //LIBNET_DISTRIBUTION_SELECTIONKEY_H
