#include <evcpp/TcpServer.h>
#include <evcpp/CircularBuffer.h>
#include <evcpp/Connection.h>
#include <evcpp/ByteBuffer.h>

using namespace ev;

class EchoServer
{
public:
    EchoServer(int port, int threads)
        : server_(port, threads)
    {
        ReadMessageCallback cb = [this](ConnectionPtr conn, ByteBuffer* buf, uint64_t timestamp) {
            this->hande_read(conn, buf, timestamp);
        };

        server_.read_message_callback(cb);
    }

    void run()
    {
        server_.run();
    }

private:
    void hande_read(ConnectionPtr conn, ByteBuffer* buf, uint64_t timestamp)
    {
        conn->write(buf->data(), buf->remaining());
    }

    TcpServer server_;
};

int main(int argc, char* argv[])
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