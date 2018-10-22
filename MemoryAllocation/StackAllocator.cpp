#include "StackAllocator.h"
#include <assert.h>

namespace Ultrality
{
	namespace Memory
	{

		StackAllocator::StackAllocator(const size_t& size, void* start)
			: Allocator(size, start), m_current_pos(start)
		{
			assert(size > 0);

			#if _DEBUG
			m_prev_position = nullptr;
			#endif
		}

		StackAllocator::~StackAllocator()
		{
			#if _DEBUG
			m_prev_position = nullptr;
			#endif

			m_current_pos = nullptr;
		}

		void* StackAllocator::allocate(const size_t& size, const unsigned __int8& alignment)
		{
			assert(size != 0);

			unsigned __int8 adjustment = pointer_math::alignForwardAdjustmentWithHeader(m_current_pos, alignment, sizeof(AllocationHeader));

			if (m_usedMemory + adjustment + size > m_size)
				return nullptr;

			void* aligned_address = pointer_math::add(m_current_pos, adjustment);

			//Add Allocation Header
			AllocationHeader* header = (AllocationHeader*)(pointer_math::subtract(aligned_address, sizeof(AllocationHeader)));

			header->m_adjustment = adjustment;

			#if _DEBUG
			header->m_prev_address = m_prev_position;

			m_prev_position = aligned_address;
			#endif

			m_current_pos = pointer_math::add(aligned_address, size);

			m_usedMemory += size + adjustment;
			m_numOfAllocations++;

			return aligned_address;
		}

		void StackAllocator::deallocate(void* p)
		{
#ifdef DEBUG
			assert(p == m_prev_position);
#endif // DEBUG

			//Access the AllocationHeader in the bytes before p
			AllocationHeader* header = (AllocationHeader*)(pointer_math::subtract(p, sizeof(AllocationHeader)));

			m_usedMemory -= (uintptr_t)m_current_pos - (uintptr_t)p + header->m_adjustment;

			m_current_pos = pointer_math::subtract(p, header->m_adjustment);

			#if _DEBUG
			m_prev_position = header->m_prev_address;
			#endif

			m_numOfAllocations--;
		}
	}
}