#include "CircularBuffer.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <algorithm>

#include "Logger.h"

namespace net
{

CircularBuffer::CircularBuffer(uint32_t capacity)
    :in_(0),
     out_(0)
{
  uint32_t cap = capacity > 0x80000000 ? 0x80000000 : capacity; //2^31

  if (is_pow_of_two(cap)) {
    capacity_ = cap;
  } else {
    capacity_ = roundup_pow_of_two(cap);
  }

  data_ = (char*) malloc(capacity_);
}


CircularBuffer::~CircularBuffer()
{
  free(data_);
}

void CircularBuffer::put(void* buffer, uint32_t length)
{
  if (buffer_remaining() < length) {
    resize(buffer, length);
    return;
  }
  else {
    //length <= buffer_remaining()
    uint32_t in_index = in();

    /* first put the data starting from in_ to buffer end */
    uint32_t len = std::min(length, capacity_ - in_index);
    memcpy(data_ + in_index, buffer, len);

    /* then put the rest (if any) at the beginning of the buffer */
    memcpy(data_, (char *) buffer + len, length - len);

    in_ += length;
  }
}


uint32_t CircularBuffer::get(void* buffer, uint32_t length)
{
  uint32_t size  = std::min(length, buffer_len());

  uint32_t out_index = out();

  /* first get the data from out_ until the end of the buffer */
  uint32_t len = std::min(size, capacity_ - out_index);
  memcpy(buffer, data_ + out_index, len);

  /* then get the rest (if any) from the beginning of the buffer */
  memcpy((char*)buffer + len, data_, size - len);

  out_ += size;
  return size;
}

void CircularBuffer::resize(void* buffer, uint32_t length)
{
  uint32_t cap = roundup_pow_of_two(length + capacity_);
  //LOG_INFO("CircularBuffer::resize %d", cap);
  char* data = (char*)malloc(cap);

  uint32_t buff_len = buffer_len();
  uint32_t out_index = out();
  uint32_t in_index = in();

  uint32_t len = std::min(buffer_remaining(), capacity_ - out_index);

  // first put the data starting from in_ to buffer end
  memcpy(data, data_ + out_index, len);

  // then put the rest (if any) from the beginning of the buffer to out_
  memcpy(data + len, data_, in_index);

  //last append data to the buffer
  memcpy(data + buff_len, buffer, length);


  free(data_);
  data_ = data;

  out_ = 0;
  in_ = length + buff_len;
  capacity_ = cap;
}




}
