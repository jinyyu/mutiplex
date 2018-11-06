#include "evcpp/Connection.h"
#include "evcpp/EventLoop.h"
#include "evcpp/ByteBuffer.h"
#include "evcpp/CircularBuffer.h"
#include "evcpp/Timestamp.h"
#include "Debug.h"
#include "EventSource.h"

#include <unistd.h>
#include <assert.h>

namespace ev
{

Connection::Connection(int fd,
                       EventLoop* loop,
                       const InetSocketAddress& local,
                       const InetSocketAddress& peer)
    : event_source_(nullptr),
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
    if (event_source_) {
        delete (event_source_);
    }
    ::close(fd_);

    if (buffer_out_) {
        delete buffer_out_;
    }

    LOG_DEBUG("connection closed %d", fd_);

}

void Connection::register_event()
{
    event_source_ = new EventSource(fd_, loop_);

    event_source_->enable_reading();
    event_source_->set_reading_callback([this](uint64_t timestamp) {
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
            event_source_->disable_reading();
            close();
        }
        else {
            buffer->position(n);
            buffer->flip();

            if (read_message_callback_) {
                read_message_callback_(shared_from_this(), buffer, timestamp);
            }
        }
    });


    event_source_->set_writing_callback([this](uint64_t timestamp) {
        handle_write(timestamp);
    });

    event_source_->set_error_callback([this](uint64_t timestamp) {
        force_close();
    });
    loop_->register_event(event_source_);

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
    loop_->post_callback(cb);
}

void Connection::force_close()
{
    auto cb = [self = shared_from_this()]() {
        self->state_ = Closed;
        self->buffer_out_->clear();
        self->event_source_->interest_ops(0);
        self->loop_->unregister_event(self->event_source_);

        if (self->loop_->connections_.find(self->fd_) != self->loop_->connections_.end()) {

            if (self->connection_closed_callback_) {
                self->connection_closed_callback_(self, Timestamp::current());
            }

            self->loop_->remove_connection(self->fd_);
        }

    };

    loop_->post_callback(cb);
}

bool Connection::write(const ByteBufferPtr& buffer)
{
    if (is_closed()) {
        return false;
    }
    loop_->post_callback([self = shared_from_this(), buffer] {
        self->do_write(buffer->data(), buffer->remaining());
    });
    return true;
}

bool Connection::write(const void* data, uint32_t len)
{
    if (is_closed()) {
        return false;
    }
    ByteBufferPtr buffer(new ByteBuffer(len));
    buffer->put(data, len);
    buffer->flip();
    loop_->post_callback([this, buffer]() {
        this->do_write(buffer->data(), static_cast<uint32_t>(buffer->remaining()));
    });
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
        event_source_->enable_writing();
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
        event_source_->disable_writing();
        close();
        return;
    }

    else if (buffer_out_->empty()) {
        event_source_->disable_writing();
        return;
    }
}

bool Connection::has_bytes_to_write() const
{
    return buffer_out_ && !buffer_out_->empty();
}

}
