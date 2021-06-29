#pragma once

#include "Particle.h"
#include <cstdint>

struct Block
{
  bool m_Occupied;
  Block* m_PreviousBlock; //free + used block
  size_t m_BlockSize; // free + used block
  union 
  {
    Block* m_NextFreeBlock; //free block
    Block* m_TopOfBlockPtr; //used block
  };
};

struct HeapHeader
{
  Block* m_FreeHeader;
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

  Block* ParticleHeap::FindBlock();
  Block* ParticleHeap::SplitBlock(size_t size);

  ParticleHeap();
  ~ParticleHeap();
  void* ParticleAlloc(size_t size, size_t alignment);
  void ParticleFree(void* ptr);

  constexpr int GetParticleSize() { return particle_size; };
  constexpr int GetMaxPatricles() { return max_particles - 1; };
  constexpr int GetMaxHeapSize() { return max_heap_size; };

private:
  static constexpr int particle_size = sizeof(Particle);
  static constexpr int max_particles = 30 * 1000;
  static constexpr int heap_overhead = 1024 * 512;
  static constexpr int max_heap_size = max_particles * particle_size + heap_overhead;

  static constexpr int block_header = sizeof(bool) + sizeof(Block*);
  static constexpr int block_size = block_header + particle_size;

  void* buffer_ptr;
  HeapHeader* heap_head;
};