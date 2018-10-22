#include "Allocator.h"
#include <assert.h>

namespace Ultrality
{
	namespace Memory
	{
		Allocator::Allocator(const size_t& size, void* start)
		{
			this->m_size = size;
			this->m_start = start;
			this->m_usedMemory = 0;
			this->m_numOfAllocations = 0;
		}
		Allocator::~Allocator()
		{
			assert(m_numOfAllocations == 0 && m_usedMemory == 0);

			m_start = nullptr;
			m_size = 0;
		}

		void* Allocator::allocate(const size_t& size, const unsigned __int8& allignment)
		{
			return 0;
		}
		void Allocator::deallocate(void* address)
		{
			
		}

		void* Allocator::getStart() const
		{
			return m_start;
		}
		size_t Allocator::getSize() const
		{
			return m_size;
		}
		size_t Allocator::getUsedMemory() const
		{
			return m_usedMemory;
		}
		size_t Allocator::getNumAllocations() const
		{
			return m_numOfAllocations;
		}
	}
}