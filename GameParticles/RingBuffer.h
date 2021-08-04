#pragma once

#include "Particle.h"

class RingBuffer
{
public:
  RingBuffer(size_t size);
  ~RingBuffer();
  Particle* Allocate();
  void Remove();
  bool IsEmpty() const { return active_count == 0; }
  bool IsFull() const { return active_count == max_array_size; }

  Particle* GetAt(int index) const { return &buffer_array[(oldest_active_index + index) % max_array_size]; }

  int GetActiveCount() const { return active_count; }

private:
  int oldest_active_index;
  int active_count;
  int	max_array_size;

  Particle* buffer_array;
};