#include <evcpp/TcpServer.h>
#include <evcpp/CircularBuffer.h>
#include <evcpp/Connection.h>
#include <evcpp/ByteBuffer.h>
#include <evcpp/Timestamp.h>

using namespace ev;

#define NUM_THREADS 2

class EchoServer
{
public:
    EchoServer(int port)
        : server_(port, NUM_THREADS)
    {
        ConnectionEstablishedCallback cb = [this](ConnectionPtr conn, uint64_t timestamp) {
            this->hande_conn(conn, timestamp);
        };

        server_.connection_established_callback(cb);
    }

    void run()
    {
        server_.run();
    }

private:
    void hande_conn(ConnectionPtr conn, uint64_t timestamp)
    {
        std::string str = ev::Timestamp::to_string(timestamp);
        str.push_back('\n');

        conn->write(str.c_str(), str.size());
        conn->close();
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
