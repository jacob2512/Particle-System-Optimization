#pragma once

class RingBuffer
{
public:
  RingBuffer();
  ~RingBuffer();
  void Add();
  void Remove();
private:
  //use these in the ring buffer to track active index
  int oldest_active_index;
  int next_inactive_index;
};