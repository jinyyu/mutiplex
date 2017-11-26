#include <net4cxx/TcpServer.h>
#include <net4cxx/ByteBuffer.h>
#include <net4cxx/Connection.h>
#include <net4cxx/Timestamp.h>

using namespace net4cxx;

void connection_com(ConnectionPtr conn, const Timestamp &timestamp)
{
    printf("new connection time = %s\n", timestamp.to_string().c_str());
}

void closed(ConnectionPtr conn, const Timestamp &timestamp)
{
    printf("connection closed   = %s\n", timestamp.to_string().c_str());
}

void read_cb(ConnectionPtr conn, ByteBuffer *buf, const Timestamp &)
{
    conn->write(buf->data(), static_cast<uint32_t>(buf->remaining()));
}

int main(int argc, char *argv[])
{
    TcpServer tcpServer(2048, 2);
    tcpServer.connection_established_callback(connection_com);
    tcpServer.connection_closed_callback(closed);
    tcpServer.read_message_callback(read_cb);
    tcpServer.run();

}
