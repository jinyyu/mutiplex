#include "EventSource.h"
#include "evcpp/EventLoop.h"

namespace ev
{

EventSource::~EventSource()
{
    if (state_ == StateDeleted) {
        loop_->unregister_event(this);
    }
}


void EventSource::enable_ops(uint32_t op)
{
    interest_ops_ |= op;
    if (interest_ops_ ) {
        if (state_ == StateNew || state_ == StateDeleted) {
            loop_->register_event(this);
            state_ = StateExists;
        }
        else {
            loop_->modify_event(this);
        }
    }
}

void EventSource::disable_ops(uint32_t op)
{
    interest_ops_ &= ~op;
    if (interest_ops_ == 0) {
        loop_->unregister_event(this);
        state_ = StateDeleted;
    }
    else {
        loop_->modify_event(this);
    }
}

}
