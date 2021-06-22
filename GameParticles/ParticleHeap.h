#pragma once

#include "Particle.h"

struct Block
{
  bool m_Occupied;
  Block* m_NextBlockHeader;
  Particle* m_Particle;
};

struct HeapHeader
{
  Block* top_block;
  size_t m_HeapUsedSize;
};

class ParticleHeap
{
public:

  Block* FindBlock() const; 
  Block* GetBlock(Particle* p) const;

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

  HeapHeader* heap_head;
};