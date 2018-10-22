#pragma once

#include "Allocator.h"

namespace Ultrality
{
	namespace Memory
	{
		class ProxyAllocator : public Allocator
		{
		private:
			ProxyAllocator(const ProxyAllocator&); //Prevent copies because it might cause errors
			ProxyAllocator& operator=(const ProxyAllocator&);

			Allocator& m_allocator;

		public:
			ProxyAllocator(Allocator& allocator);
			~ProxyAllocator();

			void* allocate(const size_t& size, const unsigned __int8& alignment) override;

			void deallocate(void* p) override;
		};
	}
}
