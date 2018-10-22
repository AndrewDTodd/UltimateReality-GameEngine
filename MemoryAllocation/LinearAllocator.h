#pragma once

#include "Allocator.h"

namespace Ultrality
{
	namespace Memory
	{
		class LinearAllocator : public Allocator
		{
		private:
			LinearAllocator(const LinearAllocator&);
			LinearAllocator& operator=(const LinearAllocator&);

			void* m_curentPosition;
		public:
			LinearAllocator(const size_t& size, void* start);
			~LinearAllocator();

			void* allocate(const size_t& size, const unsigned __int8& allignment) override;
			void deallocate(void* address) override;

			void clear();
		};
	}
}