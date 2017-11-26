#include "net4cxx/SelectionKey.h"
#include "net4cxx/Channel.h"

#include <sys/epoll.h>
#include <inttypes.h>

namespace net4cxx
{

const uint32_t SelectionKey::OP_IN = EPOLLIN; //There is data to read

const uint32_t SelectionKey::OP_OUT = EPOLLOUT; //There is data to read

const uint32_t SelectionKey::OP_PRI = EPOLLPRI; //There is urgent data to read

const uint32_t SelectionKey::OP_ERR = EPOLLERR; //Error condition

const uint32_t
    SelectionKey::OP_HUP = EPOLLHUP; //Hung up (the connection has been broken, usually for pipes and sockets)


Selector* SelectionKey::selector() const
{
    return channel_->selector_;
}

int SelectionKey::fd() const
{
    return channel_->fd();
}

void SelectionKey::cancel()
{
    channel_->disable_all();
}

std::string SelectionKey::op_get_string(uint32_t op)
{
    std::string ret;
    if (op & SelectionKey::OP_IN)
        ret.append("OP_IN");
    if (op & SelectionKey::OP_OUT)
        ret.append(" OP_OUT");
    if (op & SelectionKey::OP_PRI)
        ret.append(" OP_PRI");
    if (op & SelectionKey::OP_ERR)
        ret.append(" OP_ERR");
    if (op & SelectionKey::OP_HUP)
        ret.append(" OP_HUP");

    return ret;
}

}