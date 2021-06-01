#include "ParticleHeap.h"

#include <assert.h>
#include <stdio.h>


ParticleHeap::ParticleHeap()
{
  heap_ptr = HeapCreate(HEAP_NO_SERIALIZE, particle_size, 0);
  assert(heap_ptr != nullptr);
}

ParticleHeap::~ParticleHeap()
{
  HeapDestroy(heap_ptr);
}

Particle* ParticleHeap::ParticleAlloc()
{
  return (Particle*)HeapAlloc(heap_ptr, HEAP_NO_SERIALIZE, particle_size);
}

void ParticleHeap::ParticleFree(Particle* ptr)
{
  HeapFree(heap_ptr,HEAP_NO_SERIALIZE,ptr);
}