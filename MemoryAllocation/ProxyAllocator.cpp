#include "ProxyAllocator.h"
#include <assert.h>

namespace Ultrality
{
	namespace Memory
	{
		/*ProxyAllocator& ProxyAllocator::operator=(const ProxyAllocator&)
		{

		}*/

		ProxyAllocator::ProxyAllocator(Allocator& allocator)
			: Allocator(allocator.getSize(), allocator.getStart()), m_allocator(allocator)
		{

		}

		ProxyAllocator::~ProxyAllocator()
		{

		}

		void* ProxyAllocator::allocate(const size_t& size, const unsigned __int8& alignment)
		{
			assert(size != 0);
			m_numOfAllocations++;

			size_t mem = m_allocator.getUsedMemory();

			void* p = m_allocator.allocate(size, alignment);

			m_usedMemory += m_allocator.getUsedMemory() - mem;

			return p;
		}

		void ProxyAllocator::deallocate(void* p)
		{
			m_numOfAllocations--;

			size_t mem = m_allocator.getUsedMemory();

			m_allocator.deallocate(p);

			m_usedMemory -= mem - m_allocator.getUsedMemory();
		}
	}
}