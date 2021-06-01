#pragma once

#include "Particle.h"

#include <windows.h>
#include <heapapi.h>

class ParticleHeap
{
public:

  ParticleHeap();
  ~ParticleHeap();
  Particle* ParticleAlloc();
  void ParticleFree(Particle* ptr);

private:
  constexpr static int particle_size = sizeof(Particle);
  constexpr static int max_heap_size = 30 * 1000 * particle_size;
  HANDLE heap_ptr;
};