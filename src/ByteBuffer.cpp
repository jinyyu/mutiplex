#include <string.h>
#include "ByteBuffer.h"

namespace net
{

Status ByteBuffer::position(int p)
{
  if (p <= limit_ && p >= 0) {
    position_ = p;
    if (mark_ > position_) {
      mark_ = -1;
    }
    return Status::ok();
  } else {
    return Status::invalid_argument("invalid position");
  }
}


}