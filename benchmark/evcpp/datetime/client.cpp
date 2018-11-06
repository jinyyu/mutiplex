#include <evcpp/Session.h>
#include <evcpp/EventLoop.h>
#include <evcpp/ByteBuffer.h>
#include <evcpp/Timestamp.h>
#include <evcpp/Connection.h>

using namespace ev;

void read_cb(ConnectionPtr conn, ByteBuffer* buffer, uint64_t timestamp)
{
    std::string str((char*) buffer->data(), buffer->remaining());
    printf("%s\n", str.c_str());
}

void close_cb(ConnectionPtr conn, uint64_t timestamp)
{
    conn->loop()->stop();
}

class DatetimeClient
{
public:
    DatetimeClient(const char* ip, int port)
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

    Session* session;
    const char* ip_;
    int port_;
};

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("usage %s <ip> <port>\n", argv[0]);
        return -1;
    }

    DatetimeClient client(argv[1], atoi(argv[2]));
    client.run();
}