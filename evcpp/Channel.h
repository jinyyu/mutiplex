#pragma once
#include <boost/noncopyable.hpp>
#include <evcpp/callbacks.h>


namespace ev
{
class Selector;
class SelectionKey;
class Timestamp;

class Channel: boost::noncopyable
{
public:
    explicit Channel(Selector* selector, int fd);

    ~Channel();

    void enable_reading(const SelectionCallback& callback);

    void disable_reading();

    void enable_writing();

    void disable_writing();

    void set_writing_selection_callback(const SelectionCallback& callback)
    { write_callback_ = callback; }

    void set_error_selection_callback(const SelectionCallback& callback)
    { error_callback_ = callback; }

    void reset_fd(int fd);

    void handle_read(uint64_t timestamp) const
    {
        if (read_callback_)
            read_callback_(timestamp, selection_key_);
    }

    void handle_wirte(uint64_t timestamp) const
    {
        if (write_callback_)
            write_callback_(timestamp, selection_key_);
    }

    void handle_error(uint64_t timestamp) const
    {
        if (error_callback_)
            error_callback_(timestamp, selection_key_);
    }

    void disable_all();

private:
    friend class SelectionKey;
    int fd() const
    { return fd_; }

    void register_ops();

    void unregister_ops();

private:
    enum Status
    {
        NEW = 0,
        EXISTS = 1,
        DELETED = 2
    };

    int fd_;
    Selector* selector_;
    SelectionKey* selection_key_;
    int status_;
    SelectionCallback read_callback_;
    SelectionCallback write_callback_;
    SelectionCallback error_callback_;
};

}

