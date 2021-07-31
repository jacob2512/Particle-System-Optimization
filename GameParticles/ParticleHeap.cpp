#include "ParticleHeap.h"

#include "CustomAssert.h"
#include <stdlib.h>
#include <new.h>

// Marks a variable as unused (to avoid a compile warning)
#define UNREFERENCED_VARIABLE(P) (void)(P)

// Aligns (pads) size to a given alignment
#define ALIGN_ARB(n, a) (((size_t)(n) + ((size_t)(a)-1)) & ~(size_t)((a)-1))  // 'a' needs to be a power of 2

ParticleHeap::ParticleHeap()
{
  buffer_ptr = malloc(max_heap_size);
  void* aligned_buffer_ptr = (void*)ALIGN_ARB(buffer_ptr,16);

  heap_head = (HeapHeader*)aligned_buffer_ptr;

  if (heap_head)
  {
    heap_head->m_HeapSize = max_heap_size - sizeof(HeapHeader);
    heap_head->m_HeapUsedSize = 0;
    heap_head->m_HeapFreeSize = heap_head->m_HeapSize - sizeof(BlockHeader);

    heap_head->m_FreeHeader = (BlockHeader*)heap_head->m_FirstHeapByte;
    heap_head->m_FreeHeader->m_Occupied = false;
    heap_head->m_FreeHeader->m_BlockDataSize = heap_head->m_HeapFreeSize;
    heap_head->m_FreeHeader->m_NextFreeBlock = nullptr;
    heap_head->m_FreeHeader->m_PreviousBlock = nullptr;
    heap_head->m_FreeHeader->m_PreviousFreeBlock = nullptr;
  }
}

ParticleHeap::~ParticleHeap()
{
  if (buffer_ptr != nullptr)
  {
    free(buffer_ptr);
  }
}

void* ParticleHeap::ParticleAlloc(size_t size, size_t alignment)
{
  size_t aligned_size = ALIGN_ARB(size, alignment);

  if (aligned_size > heap_head->m_HeapFreeSize) //too much, cannot allocate
  {
    return nullptr;
  }
  else //within limit, allocate
  {
    BlockHeader* current_block = heap_head->m_FreeHeader;

    while (current_block != nullptr && current_block->m_BlockDataSize < aligned_size)
    {
      current_block = current_block->m_NextFreeBlock;
    }

    if (current_block)
    {
      current_block->m_Occupied = true;

      if (current_block->m_BlockDataSize > aligned_size + sizeof(BlockHeader))
      {
        BlockHeader* new_small_block = (BlockHeader*)((size_t)current_block + sizeof(BlockHeader) + aligned_size);
        new_small_block->m_BlockDataSize = current_block->m_BlockDataSize - sizeof(BlockHeader) - aligned_size;
        new_small_block->m_Occupied = false;
        new_small_block->m_PreviousBlock = current_block;
        new_small_block->m_PreviousFreeBlock = current_block;
        new_small_block->m_NextFreeBlock = current_block->m_NextFreeBlock;

        current_block->m_NextFreeBlock = new_small_block;
        current_block->m_BlockDataSize = aligned_size;

        heap_head->m_HeapFreeSize -= sizeof(BlockHeader);

        BlockHeader* next_block = (BlockHeader*)((size_t)new_small_block + sizeof(BlockHeader) + new_small_block->m_BlockDataSize);
        
        next_block->m_PreviousBlock = new_small_block;
      }

      heap_head->m_HeapUsedSize += current_block->m_BlockDataSize;
      heap_head->m_HeapFreeSize -= current_block->m_BlockDataSize;

      if (current_block->m_PreviousFreeBlock == nullptr) //first block
      {
        heap_head->m_FreeHeader = current_block->m_NextFreeBlock;
        if (current_block->m_NextFreeBlock)
        {
          current_block->m_NextFreeBlock->m_PreviousFreeBlock = nullptr;
        }
      }
      else
      {
        current_block->m_PreviousFreeBlock->m_NextFreeBlock = current_block->m_NextFreeBlock;
        if (current_block->m_NextFreeBlock)
        {
          current_block->m_NextFreeBlock->m_PreviousFreeBlock = current_block->m_PreviousFreeBlock;
        }
      }

      void* aligned_address = (void*)ALIGN_ARB((size_t)current_block + sizeof(BlockHeader), alignment);
      *(BlockHeader**)((size_t)aligned_address - sizeof(BlockHeader*)) = current_block;

      return aligned_address;
    }
  }

  return nullptr;
}

void ParticleHeap::ParticleFree(void* ptr)
{
  BlockHeader* block_to_free = *(BlockHeader**)((size_t)ptr - sizeof(BlockHeader*));

  if (block_to_free)
  {
    block_to_free->m_Occupied = false;

    heap_head->m_HeapFreeSize += block_to_free->m_BlockDataSize;
    heap_head->m_HeapUsedSize -= block_to_free->m_BlockDataSize;

    BlockHeader* next_block = (BlockHeader*)((size_t)block_to_free + sizeof(BlockHeader) + block_to_free->m_BlockDataSize);

    //we are at the first block
    if ((size_t)next_block >= (size_t)(heap_head->m_FirstHeapByte + heap_head->m_HeapSize))
    {
      next_block = nullptr;
    }

    //check neighboring blocks 
    
    //merge previous free block
    if (block_to_free->m_PreviousBlock && !block_to_free->m_PreviousBlock->m_Occupied)
    {
      block_to_free->m_PreviousBlock->m_BlockDataSize += sizeof(BlockHeader) + block_to_free->m_BlockDataSize;
      heap_head->m_HeapFreeSize += sizeof(BlockHeader);

      if (next_block)
      {
        next_block->m_PreviousBlock = block_to_free->m_PreviousBlock;
      }
      block_to_free = block_to_free->m_PreviousBlock;
    }
    else //merge new free block to front of free header
    {
      block_to_free->m_PreviousFreeBlock = nullptr;
      block_to_free->m_NextFreeBlock = heap_head->m_FreeHeader;
      heap_head->m_FreeHeader = block_to_free;
    }

    //merge next free block
    if (next_block && !next_block->m_Occupied)
    {
      block_to_free->m_BlockDataSize += sizeof(BlockHeader) + next_block->m_BlockDataSize;
      heap_head->m_HeapFreeSize += sizeof(BlockHeader);

      BlockHeader* next_next_block = (BlockHeader*)((size_t)next_block + sizeof(BlockHeader) + next_block->m_BlockDataSize);
      next_next_block->m_PreviousBlock = next_block->m_PreviousBlock;

      if (next_block->m_PreviousFreeBlock)
      {
        next_block->m_PreviousFreeBlock->m_NextFreeBlock = next_block->m_NextFreeBlock;
      }
      else
      {
        heap_head->m_FreeHeader = next_block->m_PreviousBlock;
      }

      if (next_block->m_NextFreeBlock)
      {
        next_block->m_NextFreeBlock->m_PreviousFreeBlock = next_block->m_PreviousFreeBlock;
      }
    }
  }
}