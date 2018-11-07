#include <mutiplex/TcpServer.h>
#include <mutiplex/CircularBuffer.h>
#include <mutiplex/Connection.h>
#include <mutiplex/ByteBuffer.h>

using namespace muti;

class EchoServer
{
public:
    EchoServer(const std::string& addr, int threads)
        : server_(addr, threads)
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
    if (argc != 3) {
        printf("usage %s <addr> <threads>\n", argv[0]);
        return -1;
    }
    //set_log_level(Logger::ERROR);

    int threads = atoi(argv[2]);

    EchoServer server(argv[1], threads);
    server.run();
}