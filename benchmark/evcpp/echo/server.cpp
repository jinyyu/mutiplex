#include <evcpp/TcpServer.h>
#include <evcpp/CircularBuffer.h>
#include <evcpp/Connection.h>
#include <evcpp/ByteBuffer.h>

using namespace ev;

#define NUM_THREADS 1

class EchoServer
{
public:
    EchoServer(int port)
        : server_(port, NUM_THREADS)
    {
        ReadCallback cb = [this](ConnectionPtr conn, ByteBuffer* buf, uint64_t timestamp) {
            this->hande_read(conn, buf, timestamp);
        };

        server_.set_read_callback(cb);
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
    if (argc != 2) {
        printf("usage %s <port>\n", argv[0]);
        return -1;
    }

    int port = atoi(argv[1]);

    EchoServer server(port);
    server.run();
}