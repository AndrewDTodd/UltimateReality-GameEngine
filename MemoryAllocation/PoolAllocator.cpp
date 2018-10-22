#include "PoolAllocator.h"
#include <assert.h>

namespace Ultrality
{
	namespace Memory
	{
		/*PoolAllocator& PoolAllocator::operator=(const PoolAllocator&)
		{

		}*/

		PoolAllocator::PoolAllocator(const size_t& objectSize, const unsigned __int8& objectAlignment, const size_t& size, void* mem)
			: Allocator(size, mem), m_objectSize(objectSize), m_objectAlignment(objectAlignment)
		{
			assert(objectSize >= sizeof(void*));

			//Calculate adjustment needed to keep object correctly aligned
			unsigned __int8 adjustment = pointer_math::alignForwardAdjustment(mem, objectAlignment);

			m_free_list = (void**)pointer_math::add(mem, adjustment);

			size_t numObjects = (size - adjustment) / objectSize;

			void** p = m_free_list;

			//Initialize free blocks list
			for (size_t i = 0; i < numObjects - 1; i++)
			{
				*p = pointer_math::add(p, objectSize);
				p = (void**)*p;
			}

			*p = nullptr;
		}
		PoolAllocator::PoolAllocator(const PoolAllocator& allocator, void* mem)
			: Allocator(allocator.m_size, mem), m_objectSize(allocator.m_objectSize), m_objectAlignment(allocator.m_objectAlignment)
		{
			assert(allocator.m_objectSize >= sizeof(void*));

			//Calculate adjustment needed to keep object correctly aligned
			unsigned __int8 adjustment = pointer_math::alignForwardAdjustment(mem, allocator.m_objectAlignment);

			m_free_list = (void**)pointer_math::add(mem, adjustment);

			size_t numObjects = (allocator.m_size - adjustment) / allocator.m_objectSize;

			void** p = m_free_list;

			//Initialize free blocks list
			for (size_t i = 0; i < numObjects - 1; i++)
			{
				*p = pointer_math::add(p, allocator.m_objectSize);
				p = (void**)*p;
			}

			*p = nullptr;
		}
		PoolAllocator::PoolAllocator(PoolAllocator&& allocator, void* mem)
			: Allocator(allocator.m_size, mem), m_objectSize(allocator.m_objectSize), m_objectAlignment(allocator.m_objectAlignment)
		{
			assert(allocator.m_objectSize >= sizeof(void*));

			//Calculate adjustment needed to keep object correctly aligned
			unsigned __int8 adjustment = pointer_math::alignForwardAdjustment(mem, allocator.m_objectAlignment);

			m_free_list = (void**)pointer_math::add(mem, adjustment);

			size_t numObjects = (allocator.m_size - adjustment) / allocator.m_objectSize;

			void** p = m_free_list;

			//Initialize free blocks list
			for (size_t i = 0; i < numObjects - 1; i++)
			{
				*p = pointer_math::add(p, allocator.m_objectSize);
				p = (void**)*p;
			}

			*p = nullptr;
		}

		PoolAllocator::~PoolAllocator()
		{
			m_free_list = nullptr;
		}

		void* PoolAllocator::allocate(const size_t& size, const unsigned __int8& alignment)
		{
			assert(size == m_objectSize && alignment == m_objectAlignment);

			if (m_free_list == nullptr)
				return nullptr;

			void* p = m_free_list;

			m_free_list = (void**)(*m_free_list);

			m_usedMemory += size;
			m_numOfAllocations++;

			return p;
		}

		void PoolAllocator::deallocate(void* p)
		{
			*((void**)p) = m_free_list;

			m_free_list = (void**)p;

			m_usedMemory -= m_objectSize;
			m_numOfAllocations--;
		}
	}
}