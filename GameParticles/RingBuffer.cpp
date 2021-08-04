#include "RingBuffer.h"
#include "ParticleHeap.h"
#include <cassert>
#include <new>

RingBuffer::RingBuffer(size_t max_size)
{
  assert(max_size != 0);

  buffer_array = (Particle*)particle_heap.ParticleAlloc(particle_heap.GetParticleSize(), 16);

  oldest_active_index = 0;
  active_count = 0;
  max_array_size = max_size;
}

RingBuffer::~RingBuffer()
{
  while (!IsEmpty())
  {
    Remove();
  }

  if (buffer_array)
  {
    particle_heap.ParticleFree(buffer_array);
  }
}

Particle* RingBuffer::Allocate()
{
  if (IsFull())
  {
    return nullptr;
  }

  Particle* new_particle = new (&buffer_array[(oldest_active_index + active_count) % max_array_size]) Particle;
  active_count++;

  return new_particle;
}

void RingBuffer::Remove()
{
  if (IsEmpty())
  {
    return;
  }

  Particle* p = &buffer_array[oldest_active_index];
  p->~Particle();

  oldest_active_index = (oldest_active_index + 1) % max_array_size;
  active_count--;
}
