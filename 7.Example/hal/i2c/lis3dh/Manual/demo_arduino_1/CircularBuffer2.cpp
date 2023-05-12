/*  CircularBuffer for Raw Binary byte streams.
    Adapted from implementation by Adam Sawicki
    http://www.asawicki.info/news_1468_circular_buffer_of_raw_binary_data_in_c.html

*/
#include <Arduino.h>
#include "CircularBuffer2.h"

CircularBuffer::CircularBuffer(size_t capacity)
  : beg_index_(0)
  , end_index_(0)
  , size_(0)
  , capacity_(capacity)
{
  data_ = (byte*) malloc(capacity);
}

CircularBuffer::~CircularBuffer()
{
  free(data_);
}

size_t CircularBuffer::write(const byte *data, size_t num)
{
  if (num == 0) return 0;

  size_t capacity = capacity_;
  size_t bytes_to_write = min(num, capacity - size_);

  // Write in a single step
  if (bytes_to_write <= capacity - end_index_)
  {
    memcpy(data_ + end_index_, data, bytes_to_write);
    end_index_ += bytes_to_write;
    if (end_index_ == capacity) end_index_ = 0;
  }
  // Write in two steps
  else
  {
    size_t size_1 = capacity - end_index_;
    memcpy(data_ + end_index_, data, size_1);
    size_t size_2 = bytes_to_write - size_1;
    memcpy(data_, data + size_1, size_2);
    end_index_ = size_2;
  }

  size_ += bytes_to_write;
  return bytes_to_write;
}

size_t CircularBuffer::read(byte *data, size_t num)
{
  if (num == 0) return 0;

  size_t capacity = capacity_;
  size_t bytes_to_read = min(num, size_);

  // Read in a single step
  if (bytes_to_read <= capacity - beg_index_)
  {
    memcpy(data, data_ + beg_index_, bytes_to_read);
    beg_index_ += bytes_to_read;
    if (beg_index_ == capacity) beg_index_ = 0;
  }
  // Read in two steps
  else
  {
    size_t size_1 = capacity - beg_index_;
    memcpy(data, data_ + beg_index_, size_1);
    size_t size_2 = bytes_to_read - size_1;
    memcpy(data + size_1, data_, size_2);
    beg_index_ = size_2;
  }

  size_ -= bytes_to_read;
  return bytes_to_read;
}

size_t CircularBuffer::flush()
{
  size_t bytes_flushed = end_index_ - beg_index_;
  beg_index_ = 0;
  end_index_ = 0;
  size_      = 0;
  return bytes_flushed;
}
