#include "Channel.h"
#include "SelectionKey.h"
#include "Selector.h"

namespace net
{


Channel::Channel(Selector* selector, int fd)
    : selector_(selector),
      selection_key_( new SelectionKey(this)),
      status_(NEW)
{
}

Channel::~Channel()
{
  delete selection_key_;
}


void Channel::enable_reading(const SelectionCallback& callback)
{
  selection_key_->enable_ops(SelectionKey::OP_IN);
  read_callback_ = callback;
  register_ops();
}


void Channel::enable_writing(const SelectionCallback& callback)
{
  selection_key_->enable_ops(SelectionKey::OP_OUT);
  write_callback_ = callback;
  register_ops();

}


void Channel::disable_reading()
{
  selection_key_->disable_ops(SelectionKey::OP_IN);
  unregister_ops();
}

void Channel::enable_writing()
{
  selection_key_->disable_ops(SelectionKey::OP_OUT);
  unregister_ops();
}

void Channel::disable_all()
{
  selection_key_->interest_ops(0);
  unregister_ops();
}

void Channel::register_ops()
{
  if (status_ == NEW || status_ == DELETED) {
    selector_->add(selection_key_);
    status_ = EXISTS;
  } else {
    selector_->modify(selection_key_);
  }
}


void Channel::unregister_ops()
{
  if (selection_key_->interest_ops() == 0) {
    selector_->remove(selection_key_);
    status_ = DELETED;
  } else {
    selector_->modify(selection_key_);
  }
}




}