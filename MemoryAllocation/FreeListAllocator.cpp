#include "FreeListAllocator.h"
#include <assert.h>

namespace Ultrality
{
	namespace Memory
	{
		/*FreeListAllocator& FreeListAllocator::operator=(const FreeListAllocator&)
		{

		}*/

		FreeListAllocator::FreeListAllocator(const size_t& size, void* start)
			: Allocator(size, start), m_free_blocks((FreeBlock*)start)
		{
			assert(size > sizeof(FreeBlock));

			m_free_blocks->m_size = size;
			m_free_blocks->m_next = nullptr;
		}

		FreeListAllocator::~FreeListAllocator()
		{
			m_free_blocks = nullptr;
		}

		void* FreeListAllocator::allocate(const size_t& size, const unsigned __int8& alignment)
		{
			assert(size != 0 && alignment != 0);

			FreeBlock* prev_free_block = nullptr;
			FreeBlock* free_block = m_free_blocks;

			while (free_block != nullptr)
			{
				//Calculate adjustment needed to keep object correctly aligned
				unsigned __int8 adjustment = pointer_math::alignForwardAdjustmentWithHeader(free_block, alignment, sizeof(AllocationHeader));

				size_t total_size = size + adjustment;

				//If allocation doesn't fit in this FreeBlock, try the next
				if (free_block->m_size < total_size)
				{
					prev_free_block = free_block;
					free_block = free_block->m_next;
					continue;
				}

				static_assert(sizeof(AllocationHeader) >= sizeof(FreeBlock), "sizeof(AllocationHeader) < sizeof(FreeBlock)");

				//If allocations in the remaining memory will be impossible
				if (free_block->m_size - total_size <= sizeof(AllocationHeader))
				{
					//Increase allocation size instead of creating a new FreeBlock
					total_size = free_block->m_size;

					if (prev_free_block != nullptr)
						prev_free_block->m_next = free_block->m_next;
					else
						m_free_blocks = free_block->m_next;
				}
				else
				{
					//Else create a new FreeBlock containing remaining memory
					FreeBlock* next_block = (FreeBlock*)(pointer_math::add(free_block, total_size));
					next_block->m_size = free_block->m_size - total_size;
					next_block->m_next = free_block->m_next;

					if (prev_free_block != nullptr)
						prev_free_block->m_next = next_block;
					else
						m_free_blocks = next_block;
				}

				uintptr_t aligned_address = (uintptr_t)free_block + adjustment;

				AllocationHeader* header = (AllocationHeader*)(aligned_address - sizeof(AllocationHeader));
				header->m_size = total_size;
				header->m_adjustment = adjustment;

				m_usedMemory += total_size;
				m_numOfAllocations++;

				assert(pointer_math::alignForwardAdjustment((void*)aligned_address, alignment) == 0);

				return (void*)aligned_address;
			}

			//ASSERT(false && "Couldn't find free block large enough!");

			return nullptr;
		}

		void FreeListAllocator::deallocate(void* p)
		{
			assert(p != nullptr);

			AllocationHeader* header = (AllocationHeader*)pointer_math::subtract(p, sizeof(AllocationHeader));

			uintptr_t   block_start = reinterpret_cast<uintptr_t>(p) - header->m_adjustment;
			size_t block_size = header->m_size;
			uintptr_t   block_end = block_start + block_size;

			FreeBlock* prev_free_block = nullptr;
			FreeBlock* free_block = m_free_blocks;

			while (free_block != nullptr)
			{
				if ((uintptr_t)free_block >= block_end)
					break;

				prev_free_block = free_block;
				free_block = free_block->m_next;
			}

			if (prev_free_block == nullptr)
			{
				prev_free_block = (FreeBlock*)block_start;
				prev_free_block->m_size = block_size;
				prev_free_block->m_next = m_free_blocks;

				m_free_blocks = prev_free_block;
			}
			else if ((uintptr_t)prev_free_block + prev_free_block->m_size == block_start)
			{
				prev_free_block->m_size += block_size;
			}
			else
			{
				FreeBlock* temp = (FreeBlock*)block_start;
				temp->m_size = block_size;
				temp->m_next = prev_free_block->m_next;
				prev_free_block->m_next = temp;

				prev_free_block = temp;
			}

			if (free_block != nullptr && (uintptr_t)free_block == block_end)
			{
				prev_free_block->m_size += free_block->m_size;
				prev_free_block->m_next = free_block->m_next;
			}

			m_numOfAllocations--;
			m_usedMemory -= block_size;
		}
	}
}