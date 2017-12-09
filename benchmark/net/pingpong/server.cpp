#include <net4cxx/reactor/TcpServer.h>
#include <net4cxx/common/CircularBuffer.h>
#include <net4cxx/reactor/Connection.h>
#include <net4cxx/common/ByteBuffer.h>

using namespace net4cxx;

class EchoServer
{
public:
    EchoServer(int port, int threads)
        : server_(port, threads)
    {
        ReadMessageCallback cb = [this](ConnectionPtr conn, ByteBuffer *buf, const Timestamp &timestamp)
        {
            this->hande_read(conn, buf, timestamp);
        };

        server_.read_message_callback(cb);
    }

    void run()
    {
        server_.run();
    }

private:
    void hande_read(ConnectionPtr conn, ByteBuffer *buf, const Timestamp &)
    {
        conn->write(buf->data(), buf->remaining());
    }

    TcpServer server_;
};

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("usage %s <port> <threads>\n", argv[0]);
        return -1;
    }
    //set_log_level(Logger::ERROR);

    int port = atoi(argv[1]);
    int threads = atoi(argv[2]);

    EchoServer server(port, threads);
    server.run();
}