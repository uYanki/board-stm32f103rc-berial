/*  CircularBuffer for Raw Binary byte streams.
    Adapted from implementation by Adam Sawicki
    http://www.asawicki.info/news_1468_circular_buffer_of_raw_binary_data_in_c.html

*/
#ifndef _CIRCULAR_BUFFER_H_INCLUDED
#define _CIRCULAR_BUFFER_H_INCLUDED

class CircularBuffer
{
public:
  CircularBuffer(size_t capacity);
  ~CircularBuffer();

  size_t size() const { return size_; }
  size_t capacity() const { return capacity_; }
  // Return number of bytes written.
  size_t write(const byte *data, size_t bytes);
  // Return number of bytes read.
  size_t read(byte *data, size_t bytes);
  // Empty the buffer
  size_t flush();

private:
  size_t beg_index_, end_index_, size_, capacity_;
  byte *data_;
};

#endif /* _CIRCULAR_BUFFER_H_INCLUDED */
