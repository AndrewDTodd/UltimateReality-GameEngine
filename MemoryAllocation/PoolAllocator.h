#pragma once

#include "Allocator.h"

namespace Ultrality
{
	namespace Memory
	{
		class PoolAllocator : public Allocator
		{
		private:
			PoolAllocator& operator=(const PoolAllocator&);

			size_t     m_objectSize;
			unsigned __int8 m_objectAlignment;

			void**     m_free_list;

		public:
			PoolAllocator(const size_t& objectSize, const unsigned __int8& objectAlignment, const size_t& size, void* mem);
			PoolAllocator(const PoolAllocator& allocator, void* mem);
			PoolAllocator(PoolAllocator&& allocator, void* mem);
			~PoolAllocator();

			void* allocate(const size_t& size, const unsigned __int8& alignment) override;

			void deallocate(void* p) override;
		};
	}
}
