#include <string.h>
#include "ByteBuffer.h"

namespace net
{

void ByteBuffer::put(char* data,  uint32_t len)
{
  while (remaining() < len) {
    buff_.reserve(buff_.size() * 2);
    limit_ = (int)buff_.size();
  }
  memcpy(buff_.data() + position_, data, len);
}




}