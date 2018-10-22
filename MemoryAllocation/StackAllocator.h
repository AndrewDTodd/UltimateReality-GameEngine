#pragma once

#include "Allocator.h"

namespace Ultrality
{
	namespace Memory
	{
		class StackAllocator: public Allocator
		{
		private:
			struct AllocationHeader
			{
				#if _DEBUG
				void* m_prev_address;
				#endif
				unsigned __int8 m_adjustment;
			};

			#if _DEBUG
			void* m_prev_position;
			#endif

			void*  m_current_pos;
		public:
			StackAllocator(const size_t& size, void* start);
			~StackAllocator();

			void* allocate(const size_t& size, const unsigned __int8& alignment) override;

			void deallocate(void* p) override;

			StackAllocator(const StackAllocator&) = delete;
			StackAllocator& operator=(const StackAllocator&) = delete;
		};
	}
}