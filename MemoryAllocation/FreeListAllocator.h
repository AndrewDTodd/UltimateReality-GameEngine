#pragma once

#include "Allocator.h"

namespace Ultrality
{
	namespace Memory
	{
		class FreeListAllocator : public Allocator
		{
		private:
			struct AllocationHeader
			{
				size_t m_size;
				unsigned __int8 m_adjustment;
			};

			struct FreeBlock
			{
				size_t     m_size;
				FreeBlock* m_next;
			};

			FreeListAllocator(const FreeListAllocator&); //Prevent copies because it might cause errors
			FreeListAllocator& operator=(const FreeListAllocator&);

			FreeBlock* m_free_blocks;

		public:
			FreeListAllocator(const size_t& size, void* start);
			~FreeListAllocator();

			void* allocate(const size_t& size, const unsigned __int8& alignment) override;

			void deallocate(void* p) override;
		};
	}
}
