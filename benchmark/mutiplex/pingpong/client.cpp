#include <mutiplex/Connector.h>
#include <mutiplex/EventLoop.h>
#include <mutiplex/InetAddress.h>
#include <sys/timerfd.h>
#include <mutiplex/Timestamp.h>
#include <sys/time.h>
#include <mutiplex/ByteBuffer.h>
#include <mutiplex/Connection.h>
#include <unistd.h>
#include <thread>
#include <mutiplex/InetAddress.h>

using namespace muti;

std::vector<uint64_t> totals;

class Client
{
public:
    Client(const InetAddress& addr, int session_count, int timeout, int block_size, int index)
        : server_addr_(addr),
          session_count_(session_count),
          timeout_(timeout),
          block_size_(block_size),
          total_(0),
          index_(index)
    {
        loop_.allocate_receive_buffer(6 * 1024 * 1024);
        buffer_ = malloc(block_size);

        setup_sessions();

    }

    ~Client()
    {
        for (Connector* session : sessions_) {
            delete (session);
        }

        free(buffer_);
    }

    void print_result()
    {
        uint64_t mb = total_ / 1024 / 1024 / timeout_;
        totals[index_] = mb;
    }

    void run()
    {
        loop_.run();
    }

private:

    void handle_timeout(uint64_t timestamp)
    {
        printf("stop \n");
        loop_.stop();

    }

    void setup_sessions()
    {
        for (int i = 0; i < session_count_; ++i) {
            Connector* session = new Connector(&loop_, server_addr_);

            ReadCallback read_cb = [this](ConnectionPtr conn, ByteBuffer* buffer, uint64_t timestamp) {
                this->read_callback(conn, buffer, timestamp);
            };
            session->set_read_callback(read_cb);
            EstablishedCallback connect_cb = [this](ConnectionPtr conn, uint64_t timestamp) {
                this->on_connect_success(conn);
            };
            session->established_callback(connect_cb);

            session->start_connect();
            sessions_.push_back(session);
        }
    }

    void on_connect_success(ConnectionPtr conn)
    {
        conn->write(buffer_, block_size_);
    }

    void read_callback(ConnectionPtr conn, ByteBuffer* buffer, uint64_t timestamp)
    {
        //LOG_INFO("read n = %d", buffer->remaining());
        int len = buffer->remaining();
        if (conn->write(buffer->data(), len)) {
            total_ += len;
        }
    }

private:
    EventLoop loop_;
    int session_count_;
    int timeout_;
    int block_size_;
    InetAddress server_addr_;

    std::vector<Connector*> sessions_;

    void* buffer_;

    uint64_t total_;
    int index_;

};

int main(int argc, char* argv[])
{
    if (argc != 6) {
        printf("usage %s <addr>  <session_count> <timeout> <block_size> <threads>\n", argv[0]);
        return -1;
    }
    const char* addr = argv[1];
    int session_count = std::atoi(argv[2]);
    int timeout = std::atoi(argv[3]);
    int block_size = std::atoi(argv[4]);
    int threads = std::atoi(argv[5]);

    InetAddress address(addr);

    printf("addr = %s, session_count = %d, timeout = %d, block_size = %d, threads = %d\n",
           addr,
           session_count,
           timeout,
           block_size,
           threads);

    totals.resize(threads);

    int per_thread = session_count / threads + 1;

    std::vector<std::thread> thread_vec;
    for (int i = 0; i < threads; ++i) {
        totals[i] = 0;

        auto cb = [address, per_thread, timeout, block_size, i]() {

            Client client(address, per_thread, timeout, block_size, i);
            client.run();
            client.print_result();
        };

        thread_vec.push_back(std::thread(cb));
    }

    for (int i = 0; i < threads; ++i) {
        thread_vec[i].join();
    }

    uint64_t total = 0;
    for (int i = 0; i < totals.size(); ++i) {
        total += totals[i];
    }

    printf("total send = %lu MB/s\n", total);

}