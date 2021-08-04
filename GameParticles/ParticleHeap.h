#pragma once

#include "Particle.h"
#include <cstdint>

struct BlockHeader
{
  bool m_Occupied;
  BlockHeader* m_PreviousBlock; //free + used block
  BlockHeader* m_PreviousFreeBlock; //free block
  size_t m_BlockDataSize; // free + used block
  union 
  {
    BlockHeader* m_NextFreeBlock; //free block
    BlockHeader* m_TopOfBlockPtr; //used block
  };
};

struct HeapHeader
{
  BlockHeader* m_FreeHeader;
  size_t m_HeapUsedSize;
  size_t m_HeapFreeSize;
  size_t m_HeapSize;
#pragma warning(push)
#pragma warning(disable:4200)
  uint8_t m_FirstHeapByte[0];
#pragma warning(pop)
};

class ParticleHeap
{
public:

  ParticleHeap();
  ~ParticleHeap();
  void* ParticleAlloc(size_t size, size_t alignment);
  void ParticleFree(void* ptr);

  static constexpr int GetParticleSize() { return particle_size; };
  static constexpr int GetMaxParticles() { return max_particles; };

private:
  static constexpr int particle_size = sizeof(Particle);
  static constexpr int max_particles = 30 * 1000;
  static constexpr int heap_overhead = 1024 * 1024 + 15;
  static constexpr int max_heap_size = max_particles * particle_size + heap_overhead;

  static constexpr int block_header = sizeof(bool) + sizeof(BlockHeader*);
  static constexpr int block_size = block_header + particle_size;

  void* buffer_ptr;
  HeapHeader* heap_head;
};

extern ParticleHeap particle_heap;