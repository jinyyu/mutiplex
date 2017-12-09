#include <net4cxx/reactor/TcpServer.h>
#include <net4cxx/common/CircularBuffer.h>
#include <net4cxx/reactor/Connection.h>
#include <net4cxx/common/ByteBuffer.h>
#include <net4cxx/common/SingalHandler.h>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>

using namespace net4cxx;

static log4cxx::LoggerPtr logger(log4cxx::Logger::getRootLogger());

#define NUM_THREADS 4

class EchoServer
{
public:
    EchoServer(int port)
        : server_(port, NUM_THREADS)
    {
        ReadMessageCallback cb = [this](ConnectionPtr conn, ByteBuffer* buf, const Timestamp& timestamp)
        {
            this->hande_read(conn, buf, timestamp);
        };

        server_.read_message_callback(cb);
    }

    void run()
    {
        AddSignalHandler(SIGINT, [this](){
            server_.shutdown();
        });
        server_.run();
    }

private:
    void hande_read(ConnectionPtr conn, ByteBuffer* buf, const Timestamp&)
    {
        conn->write(buf->data(), buf->remaining());
    }

    TcpServer server_;
};

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("usage %s <port> <config>\n", argv[0]);
        return -1;
    }
    log4cxx::PropertyConfigurator::configure(argv[2]);

    LOG4CXX_INFO(logger, "start");
    int port = atoi(argv[1]);

    EchoServer server(port);
    server.run();
}