#include "EventSource.h"
#include "mutiplex/EventLoop.h"
#include "Debug.h"

namespace muti
{

EventSource::~EventSource()
{
    disable_all();
}

void EventSource::enable_ops(uint32_t op)
{
    interest_ops_ |= op;
    if (interest_ops_) {
        if (state_ == StateNoEvent) {
            state_ = StateRegistered;
            loop_->register_event(this);
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
        state_ = StateNoEvent;
    }
    else {
        loop_->modify_event(this);
    }
}

void EventSource::disable_all()
{
    interest_ops_ = 0;
    if (state_ == StateRegistered) {
        loop_->unregister_event(this);
        state_ = StateNoEvent;
    }
}

}
