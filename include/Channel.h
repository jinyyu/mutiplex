#ifndef LIBNET_DISTRIBUTION_CHANNEL_H
#define LIBNET_DISTRIBUTION_CHANNEL_H


namespace net
{



class Channel
{
public:
  Channel(int fd) : fd_(fd) {}

  int fd() const {}

private:

  friend class SelectionKey;

  int events() const { return events_; }

private:

  int fd_;
  int events_;

};


}

#endif //LIBNET_DISTRIBUTION_CHANNEL_H
