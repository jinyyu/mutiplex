#include <net4cxx/reactor/TcpServer.h>
#include <net4cxx/common/CircularBuffer.h>
#include <net4cxx/reactor/Connection.h>
#include <net4cxx/common/ByteBuffer.h>
#include <net4cxx/common/Timestamp.h>

using namespace net4cxx;

#define NUM_THREADS 2

class EchoServer
{
public:
    EchoServer(int port)
        : server_(port, NUM_THREADS)
    {
        ConnectionEstablishedCallback cb = [this](ConnectionPtr conn, const Timestamp &timestamp)
        {
            this->hande_conn(conn, timestamp);
        };

        server_.connection_established_callback(cb);
    }

    void run()
    {
        server_.run();
    }

private:
    void hande_conn(ConnectionPtr conn, const Timestamp &timestamp)
    {
        std::string str = timestamp.to_string();
        str.push_back('\n');

        conn->write(str.c_str(), str.size());
        conn->close();
    }

    TcpServer server_;
};

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("usage %s <port>\n", argv[0]);
        return -1;
    }

    int port = atoi(argv[1]);

    EchoServer server(port);
    server.run();
}
