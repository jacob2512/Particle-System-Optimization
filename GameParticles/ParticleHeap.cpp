#include "ParticleHeap.h"

#include "CustomAssert.h"
#include <stdlib.h>
#include <new.h>
#include <stdio.h>

// Marks a variable as unused (to avoid a compile warning)
#define UNREFERENCED_VARIABLE(P) (void)(P)

#define ALIGN_ARB(n, a) (((size_t)(n) + ((size_t)(a)-1)) & ~(size_t)((a)-1))  // 'a' needs to be a power of 2

//Block* ParticleHeap::FindBlock() const
//{
//  Block* block = heap_head->top_block;
//  Block* res = nullptr;
//
//  while (block && !block->m_Occupied) {
//    res = block;
//    block = block->m_NextBlockHeader;
//  }
//
//  ASSERT(res != nullptr);
//  return res;
//}
//
//Block* ParticleHeap::GetBlock(Particle* p) const
//{
//  return (Block*)(p - block_header);
//}

ParticleHeap::ParticleHeap()
{
  buffer_ptr = malloc(max_heap_size + 15);
  void* aligned_buffer_ptr = (void*)ALIGN_ARB(buffer_ptr,16);

  heap_head = (HeapHeader*)aligned_buffer_ptr;

  ASSERT(heap_head != nullptr);

  heap_head->m_HeapSize = max_heap_size - sizeof(HeapHeader);
  heap_head->m_HeapUsedSize = 0;
  heap_head->m_HeapFreeSize = heap_head->m_HeapSize - sizeof(Block);

  heap_head->m_FreeHeader = (Block*)heap_head->m_FirstHeapByte;
  heap_head->m_FreeHeader->m_Occupied = false;
  heap_head->m_FreeHeader->m_BlockSize = heap_head->m_HeapFreeSize;
  heap_head->m_FreeHeader->m_NextFreeBlock = nullptr;
  heap_head->m_FreeHeader->m_PreviousBlock = nullptr;
}

ParticleHeap::~ParticleHeap()
{
  if (buffer_ptr != nullptr)
  {
    free(buffer_ptr);
  }
}

Block* ParticleHeap::FindBlock()
{
  Block* b = nullptr;
  return b;
}

Block* ParticleHeap::SplitBlock(size_t size)
{
  Block* b = nullptr;
  return b;
}

void* ParticleHeap::ParticleAlloc(size_t size, size_t alignment)
{
  size_t aligned_size = ALIGN_ARB(size, alignment);

  //printf("aligned size: %d\n", aligned_size);
  //printf("free size: %d\n", heap_head->m_HeapFreeSize);

  if (aligned_size > heap_head->m_HeapFreeSize) //too much, cannot allocate
  {
    return nullptr;
  }
  else //within limit, allocate
  {
    Block* current_block = heap_head->m_FreeHeader;
    current_block->m_Occupied = false;
    current_block->m_BlockSize = sizeof(Block) + 15;
    current_block->m_PreviousBlock = current_block;
    current_block->m_TopOfBlockPtr = nullptr;

    while (current_block->m_NextFreeBlock != nullptr)
    {
     if(current_block->m_Occupied || current_block->m_BlockSize < aligned_size)
      {
        current_block = current_block->m_NextFreeBlock;
      }
    }

    if (current_block)
    {
      heap_head->m_HeapUsedSize += aligned_size;
      heap_head->m_HeapFreeSize -= aligned_size;

      current_block->m_Occupied = true;
      current_block->m_TopOfBlockPtr = current_block;
      char* p = (char*)(&heap_head->m_FreeHeader);
      p += current_block->m_BlockSize;
      heap_head->m_FreeHeader = (Block*)p;
      return current_block;
    }
  }

  return nullptr;
}

void ParticleHeap::ParticleFree(void* ptr)
{
  Block* block_to_free = ((Block*)ptr)->m_TopOfBlockPtr;

  if (block_to_free)
  {
    //block_to_free->m_Occupied = false;

    //size_t aligned_size = ALIGN_ARB(block_to_free->m_BlockSize, 16);
    //heap_head->m_HeapFreeSize += aligned_size;
    //heap_head->m_HeapUsedSize -= aligned_size;

    heap_head->m_FreeHeader->m_NextFreeBlock = block_to_free;
  }

  ((Particle*)ptr)->~Particle();
}