#include "ParticleHeap.h"

#include "CustomAssert.h"
#include <new.h>
#include <stdlib.h>

// Marks a variable as unused (to avoid a compile warning)
#define UNREFERENCED_VARIABLE(P) (void)(P)

#define BLOCK_HEADER sizeof(bool) + sizeof(Block*)
#define BLOCK_SIZE BLOCK_HEADER + sizeof(Particle)

Block* ParticleHeap::FindBlock() const
{
  Block* block = heap_head->top_block;
  Block* res = nullptr;

  while (block && !block->m_Occupied) {
    res = block;
    block = block->m_NextBlockHeader;
  }

  ASSERT(res != nullptr);
  return res;
}

Block* ParticleHeap::GetBlock(Particle* p) const
{
  return (Block*)(p - BLOCK_HEADER);
}

ParticleHeap::ParticleHeap()
{
  heap_head = (HeapHeader*)malloc(max_heap_size);
  ASSERT(heap_head != nullptr);
  heap_head->top_block = nullptr;
}

ParticleHeap::~ParticleHeap()
{
  if (heap_head != nullptr)
  {
    free(heap_head);
  }
}

Particle* ParticleHeap::ParticleAlloc() const
{

  Particle* new_particle = new(malloc(particle_size)) Particle;

  Block* new_block = new(malloc(BLOCK_SIZE)) Block;

  if (heap_head->top_block != nullptr)
  {
    new_block = FindBlock();
    //new_block->m_Occupied = true;
    //new_block->m_NextBlockHeader = nullptr;
    //new_block->m_Particle = new_particle;
  }
  else
  {
    heap_head->top_block = new_block;
  }

  heap_head->m_HeapUsedSize += BLOCK_SIZE;

  return new_particle;

}

void ParticleHeap::ParticleFree(Particle* p) const
{
  p->~Particle();

  //Block* block_to_free = GetBlock(p);
  //block_to_free->m_Occupied = false;

  heap_head->m_HeapUsedSize -= BLOCK_SIZE;
  free(p);
}