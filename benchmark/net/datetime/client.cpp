#include <net4cxx/reactor/Session.h>
#include <net4cxx/reactor/EventLoop.h>
#include <net4cxx/common/ByteBuffer.h>
#include <net4cxx/common/Timestamp.h>
#include <net4cxx/reactor/Connection.h>

using namespace net4cxx;

void read_cb(ConnectionPtr conn, ByteBuffer *buffer, const Timestamp &timestamp)
{
    std::string str((char *) buffer->data(), buffer->remaining());
    printf("%s\n", str.c_str());
}

void close_cb(ConnectionPtr conn, const Timestamp &timestamp)
{
    conn->loop()->stop();
}

class DatetimeClient
{
public:
    DatetimeClient(const char *ip, int port)
        : ip_(ip), port_(port)
    {
        loop.allocate_receive_buffer(10240);

        InetSocketAddress local = InetSocketAddress();
        session = new Session(&loop, local);

        session->read_message_callback(read_cb);
        session->connection_closed_callback(close_cb);

    }

    ~DatetimeClient()
    {
        delete (session);
    }

    void run()
    {
        InetSocketAddress peer(ip_, port_);
        session->connect(peer);

        loop.run();
    };

private:
    EventLoop loop;

    Session *session;
    const char *ip_;
    int port_;
};

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("usage %s <ip> <port>\n", argv[0]);
        return -1;
    }

    DatetimeClient client(argv[1], atoi(argv[2]));
    client.run();
}