#include "LinearAllocator.h"
#include <assert.h>

namespace Ultrality
{
	namespace Memory
	{
		/*LinearAllocator& LinearAllocator::operator=(const LinearAllocator&)
		{

		}*/

		LinearAllocator::LinearAllocator(const size_t& size, void* start)
			:Allocator(size, start), m_curentPosition(start)
		{
			assert(size > 0);
		}

		LinearAllocator::~LinearAllocator()
		{
			this->m_curentPosition = nullptr;
		}

		void* LinearAllocator::allocate(const size_t& size, const unsigned __int8& alignment)
		{
			assert(size != 0);

			unsigned __int8 adjustment = pointer_math::alignForwardAdjustment(this->m_curentPosition, alignment);

			if (this->m_usedMemory + adjustment + size > this->m_size)
				return nullptr;

			uintptr_t aligned_address = (uintptr_t)this->m_curentPosition + adjustment;

			this->m_curentPosition = (void*)(aligned_address + size);

			this->m_usedMemory += size + adjustment;
			this->m_numOfAllocations++;

			return (void*)aligned_address;
		}

		void LinearAllocator::deallocate(void* p)
		{
			assert(false && "Use clear() instead");
		}

		void LinearAllocator::clear()
		{
			this->m_numOfAllocations = 0;
			this->m_usedMemory = 0;

			this->m_curentPosition = this->m_start;
		}
	}
}