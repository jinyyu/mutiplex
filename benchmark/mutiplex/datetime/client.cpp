#include <mutiplex/Connector.h>
#include <mutiplex/EventLoop.h>
#include <mutiplex/ByteBuffer.h>
#include <mutiplex/Timestamp.h>
#include <mutiplex/Connection.h>

using namespace muti;

void read_cb(ConnectionPtr conn, ByteBuffer* buffer, uint64_t timestamp)
{
    printf("%s->%s\n", conn->local_address().to_string().c_str(), conn->peer_address().to_string().c_str());
    std::string str((char*) buffer->data(), buffer->remaining());
    printf("%s", str.c_str());
    conn->close();
}

class DatetimeClient
{
public:
    DatetimeClient(const char* addr)
        : addr_(addr)
    {
        loop.allocate_receive_buffer(10240);

        InetAddress inet_addr(addr_);
        session = new Connector(&loop, inet_addr);

        session->established_callback([](ConnectionPtr conn, uint64_t timestamp){
            conn->set_read_callback(read_cb);
            conn->set_closed_callback([conn](ConnectionPtr conn, uint64_t timestamp){
                conn->loop()->stop();
            });
        });

    }

    ~DatetimeClient()
    {
        delete (session);
    }

    void run()
    {
        session->start_connect();

        loop.run();
    };

private:
    EventLoop loop;

    Connector* session;
    const char* addr_;
};


int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("usage %s <addr>\n", argv[0]);
        return -1;
    }

    DatetimeClient client(argv[1]);
    client.run();
}