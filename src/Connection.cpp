#include "evcpp/Connection.h"
#include "evcpp/Channel.h"
#include "evcpp/EventLoop.h"
#include "evcpp/SelectionKey.h"
#include "evcpp/ByteBuffer.h"
#include "evcpp/CircularBuffer.h"
#include "evcpp/Timestamp.h"
#include "Debug.h"

#include <unistd.h>
#include <assert.h>

namespace ev
{

Connection::Connection(int fd,
                       EventLoop* loop,
                       const InetSocketAddress& local,
                       const InetSocketAddress& peer)
    : channel_(nullptr),
      fd_(fd),
      peer_(peer),
      local_(local),
      loop_(loop),
      state_(New),
      buffer_size_(1024),
      buffer_out_(nullptr)
{
    LOG_DEBUG("new connection %d", fd_);
}

Connection::~Connection()
{
    if (channel_) {
        delete (channel_);
    }
    ::close(fd_);

    if (buffer_out_) {
        delete buffer_out_;
    }

    LOG_DEBUG("connection closed %d", fd_);

}

void Connection::setup_callbacks()
{
    channel_ = new Channel(loop_->selector(), fd_);

    SelectionCallback read_cb = [this](uint64_t timestamp, SelectionKey* key) {
        ByteBuffer* buffer = loop_->recv_buffer_;
        buffer->clear();
        ssize_t n = ::read(fd_, buffer->data(), buffer->remaining());
        int err = errno;
        if (n < 0) {
            if (error_callback_) {
                error_callback_(shared_from_this(), timestamp);
            }
            force_close();
            if (err != 104) {
                LOG_DEBUG("read error %d", errno);
            }
        }
        else if (n == 0) {
            //peer shutdown read
            channel_->disable_reading();
            close();
        }
        else {
            buffer->position(n);
            buffer->flip();

            if (read_message_callback_) {
                read_message_callback_(shared_from_this(), buffer, timestamp);
            }
        }
    };

    channel_->enable_reading(read_cb);

    SelectionCallback write_cb = [this](uint64_t timestamp, SelectionKey*) {
        this->handle_write(timestamp);
    };
    channel_->set_writing_selection_callback(write_cb);

    SelectionCallback error_cb = [this](uint64_t timestamp, SelectionKey*) {
        this->force_close();
    };
    channel_->set_error_selection_callback(error_cb);

    state_ = Receiving;

}

void Connection::close()
{
    auto cb = [self = shared_from_this()] {
        if (!self->has_bytes_to_write()) {
            //closed it
            self->state_ = Closed;
            if (self->connection_closed_callback_) {
                self->connection_closed_callback_(self, Timestamp::current());
            }
            self->loop_->remove_connection(self->fd_);
        }
        else {
            //has bytes to write
            self->state_ = Disconnecting;
        }
    };
    loop_->post(cb);
}

void Connection::force_close()
{
    auto cb = [self = shared_from_this()]() {
        self->state_ = Closed;
        self->buffer_out_->clear();
        self->channel_->disable_all();

        if (self->loop_->connections_.find(self->fd_) != self->loop_->connections_.end()) {

            if (self->connection_closed_callback_) {
                self->connection_closed_callback_(self, Timestamp::current());
            }

            self->loop_->remove_connection(self->fd_);
        }

    };

    if (loop_->is_in_loop_thread()) {
        cb();
    }
    else {
        loop_->post(cb);
    }
}

bool Connection::write(const ByteBufferPtr& buffer)
{
    if (is_closed()) {
        return false;
    }

    if (loop_->is_in_loop_thread()) {
        do_write(buffer->data(), buffer->remaining());

    }
    else {
        auto callback = [self = shared_from_this(), buffer] {
            self->do_write(buffer->data(), buffer->remaining());
        };
        loop_->post(callback);
    }
    return true;
}

bool Connection::write(const void* data, uint32_t len)
{
    if (is_closed()) {
        return false;
    }
    if (loop_->is_in_loop_thread()) {
        do_write(data, len);
    }
    else {
        ByteBufferPtr buffer(new ByteBuffer(len));
        buffer->put(data, len);
        buffer->flip();

        //copy buffer
        auto callback = [this, buffer]() {
            this->do_write(buffer->data(), static_cast<uint32_t>(buffer->remaining()));
        };
        loop_->post(callback);
    }
    return true;
}

void Connection::do_write(const void* data, uint32_t len)
{
    if (state_ == Closed) {
        force_close();
    }

    else {
        if (!buffer_out_) {
            buffer_out_ = new CircularBuffer(len);
        }
        buffer_out_->put(data, len);
        channel_->enable_writing();
    }
}

void Connection::handle_write(uint64_t timestamp)
{
    assert(!buffer_out_->empty());

    int total = buffer_out_->write_to_fd(this, timestamp);
    if (total < 0) {
        force_close();

        if (error_callback_)
            error_callback_(shared_from_this(), timestamp);
        return;
    }
    else if (total == 0) {
        //close
        force_close();
        return;
    }
        //write success
    else if (state_ == Disconnecting) {
        channel_->disable_writing();
        close();
        return;
    }

    else if (buffer_out_->empty()) {
        channel_->disable_writing();
        return;
    }
}

bool Connection::has_bytes_to_write() const
{
    return buffer_out_ && !buffer_out_->empty();
}

}
