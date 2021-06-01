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
  int GetMaxPatricles() { return max_particles - 1; };

private:
  static constexpr int particle_size = sizeof(Particle);
  static constexpr int max_particles = 30 * 1000;
  static constexpr int max_heap_size = max_particles * particle_size;
  HANDLE heap_ptr;
};