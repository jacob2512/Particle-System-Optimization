#include "ParticleHeap.h"

#include "CustomAssert.h"
#include <new.h>

// Marks a variable as unused (to avoid a compile warning)
#define UNREFERENCED_VARIABLE(P) (void)(P)

ParticleHeap::ParticleHeap()
{
  heap_ptr = HeapCreate(HEAP_NO_SERIALIZE, particle_size, max_heap_size);
  ASSERT(heap_ptr != nullptr);
}

ParticleHeap::~ParticleHeap()
{
  HeapDestroy(heap_ptr);
}

Particle* ParticleHeap::ParticleAlloc() const
{
  void* pp = HeapAlloc(heap_ptr, HEAP_NO_SERIALIZE, particle_size);
  ASSERT(pp != nullptr);
  Particle* ppr = new(pp) Particle;
  return ppr;
}

void ParticleHeap::ParticleFree(Particle* ptr) const
{
  ptr->~Particle();
  bool ret = HeapFree(heap_ptr, HEAP_NO_SERIALIZE, ptr);
  UNREFERENCED_VARIABLE(ret);
  ASSERT(ret != 0);
}