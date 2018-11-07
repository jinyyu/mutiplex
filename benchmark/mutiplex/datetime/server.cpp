#include <mutiplex/TcpServer.h>
#include <mutiplex/CircularBuffer.h>
#include <mutiplex/Connection.h>
#include <mutiplex/ByteBuffer.h>
#include <mutiplex/Timestamp.h>

using namespace muti;

#define NUM_THREADS 2

class EchoServer
{
public:
    EchoServer(const std::string& addr)
        : server_(addr, NUM_THREADS)
    {
        EstablishedCallback cb = [this](ConnectionPtr conn, uint64_t timestamp) {
            this->on_new_connection(conn, timestamp);
        };

        server_.set_established_callback(cb);
    }

    void run()
    {
        server_.run();
    }

private:
    void on_new_connection(ConnectionPtr conn, uint64_t timestamp)
    {
        std::string str = muti::Timestamp::to_string(timestamp);
        str.push_back('\n');

        conn->write(str.c_str(), str.size());
        conn->close();
    }

    TcpServer server_;
};

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("usage %s <addr>\n", argv[0]);
        return -1;
    }


    EchoServer server(argv[1]);
    server.run();
}
