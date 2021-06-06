#pragma once

#include "Particle.h"

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>

class ParticleHeap
{
public:

  ParticleHeap();
  ~ParticleHeap();
  Particle* ParticleAlloc() const;
  void ParticleFree(Particle* ptr) const;
  constexpr int GetMaxPatricles() { return max_particles - 1; };

private:
  static constexpr int particle_size = sizeof(Particle);
  static constexpr int max_particles = 30 * 1000;
  static constexpr int heap_overhead = 1024 * 512;
  static constexpr int max_heap_size = max_particles * particle_size + heap_overhead;
  HANDLE heap_ptr;
};