#include <Session.h>
#include <EventLoop.h>
#include <InetSocketAddress.h>
#include <sys/timerfd.h>
#include <Logger.h>
#include <Timestamp.h>
#include <Channel.h>
#include <sys/time.h>
#include <ByteBuffer.h>
#include <Connection.h>

using namespace net;


class Client
{
public:
  Client(const InetSocketAddress& addr,int session_count, int timeout, int block_size)
      : server_addr_(addr),
        session_count_(session_count),
        timeout_(timeout),
        block_size_(block_size)
  {
    loop_.allocate_receive_buffer(6 * 1024 * 1024);
    buffer_ = malloc(block_size);

    setup_sessions();

    setup_timer();

  }

  ~Client() {
    delete(timer_channel_);
    for(Session* session : sessions_) {
      delete(session);
    }

    free(buffer_);
  }



  void run()
  {
    loop_.run();
  }

private:

  void handle_timeout(const Timestamp& timestamp)
  {
    LOG_INFO("%s", timestamp.to_string().c_str());
    loop_.stop();

  }

  void setup_timer()
  {
    int fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    if (fd < 0) {
      LOG_ERROR("timerfd_create error %d", errno);
    }

    timer_channel_ = new Channel(loop_.selector(), fd);
    SelectionCallback timeout = [this](const Timestamp & timestamp, SelectionKey *) {
      this->handle_timeout(timestamp);
    };

    timer_channel_->enable_reading(timeout);

    struct timeval timeval_now;
    struct timeval timeval_exp;
    struct timeval timeval_res;

    gettimeofday(&timeval_now, NULL);

    timeval_exp.tv_sec = timeval_now.tv_sec + timeout_;
    timeval_exp.tv_usec = timeval_now.tv_usec;

    timersub(&timeval_exp, &timeval_now, &timeval_res);

    struct itimerspec timer;
    bzero(&timer, sizeof(itimerspec));
    timer.it_value.tv_sec = timeval_res.tv_sec;
    timer.it_value.tv_nsec = timeval_res.tv_usec * 1000;

    if (timerfd_settime(fd, 0, &timer, NULL) != 0) {
      LOG_ERROR("timerfd_settime error %d", errno);
    }
  }

  void setup_sessions()
  {
    InetSocketAddress local;
    for(int i = 0; i < session_count_; ++i) {
      Session* session = new Session(&loop_, local);

      ReadMessageCallback read_cb = [this](ConnectionPtr conn, ByteBuffer* buffer, const Timestamp & timestamp) {
        this->read_callback(conn, buffer, timestamp);
      };
      session->read_message_callback(read_cb);
      ConnectionEstablishedCallback connect_cb = [this](ConnectionPtr conn, const Timestamp &){
        this->on_connect_success(conn);
      };
      session->connection_established_callback(connect_cb);

      session->connect(server_addr_);
      sessions_.push_back(session);
    }
  }

  void on_connect_success(ConnectionPtr conn)
  {
    conn->write(buffer_, block_size_);
  }

  void read_callback(ConnectionPtr conn, ByteBuffer* buffer, const Timestamp & timestamp)
  {
    LOG_INFO("read n = %d", buffer->remaining());
    conn->write(buffer->data(), buffer->has_remaining());
  }

private:
  EventLoop loop_;
  int session_count_;
  int timeout_;
  int block_size_;
  InetSocketAddress server_addr_;
  Channel* timer_channel_;

  std::vector<Session*> sessions_;

  void* buffer_;

};


int main(int argc, char* argv[])
{
  if (argc != 6) {
    printf("usage %s <ip> <port> <session_count> <timeout> <block_size>\n", argv[0]);
    return -1;
  }
  const char* ip = argv[1];
  int port = std::atoi(argv[2]);
  int session_count = std::atoi(argv[3]);
  int timeout = std::atoi(argv[4]);
  int block_size = std::atoi(argv[5]);

  InetSocketAddress address(ip, port);

  printf("ip = %s, port = %d, session_count = %d, timeout = %d, block_size = %d\n", ip, port, session_count, timeout, block_size);

  Client client(address, session_count, timeout, block_size);
  client.run();

}