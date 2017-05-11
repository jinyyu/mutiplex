#include <string.h>
#include "ByteBuffer.h"

namespace net
{

void ByteBuffer::put(char* data,  uint32_t len)
{
  while (buff_.size() - position_ < len) {
    buff_.reserve(buff_.size() * 2);
    limit_ = (uint32_t)buff_.size();
  }
  memcpy(buff_.data() + position_, data, len);
  position_ += len;
}


}