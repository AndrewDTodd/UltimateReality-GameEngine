#pragma once
#include <cstdint>
#include <assert.h>

namespace Ultrality
{
	namespace Memory
	{
		namespace pointer_math
		{
			void*       alignForward(void* address, const unsigned __int8& alignment);
			const void* alignForward(const void* address, const unsigned __int8& alignment);

			void*       alignBackward(void* address, const unsigned __int8& alignment);
			const void* alignBackward(const void* address, const unsigned __int8& alignment);

			unsigned __int8          alignForwardAdjustment(const void* address, const unsigned __int8& alignment);
			unsigned __int8          alignForwardAdjustmentWithHeader(const void* address, const unsigned __int8& alignment, const unsigned __int8& headerSize);

			unsigned __int8          alignBackwardAdjustment(const void* address, const unsigned __int8& alignment);

			void*       add(void* p, const size_t& x);
			const void* add(const void* p, const size_t& x);

			void*       subtract(void* p, const size_t& x);
			const void* subtract(const void* p, const size_t& x);
		}

		class Allocator
		{
		protected:
			void* m_start;
			size_t m_usedMemory;
			size_t m_numOfAllocations;

		public:
			size_t m_size;

			Allocator(const size_t& size, void* start);
			virtual ~Allocator();

			virtual void* allocate(const size_t& size, const unsigned __int8& allignment = 8);
			virtual void deallocate(void* address);

			void* getStart() const;
			size_t getSize() const;
			size_t getUsedMemory() const;
			size_t getNumAllocations() const;
		};

		template <class T> T* allocateNew(const size_t& size, Allocator* allocator)
		{
			void* p = allocator->allocate(size + sizeof(T), __alignof(T));
			return new (p) T(size, pointer_math::add(p, sizeof(T)));
		}

		template <class T> T* allocateNew(const size_t& size, Allocator& allocator,const T& t)
		{
			return new (allocator.allocate(sizeof(T), __alignof(T))) T(t);
		}

		template<class T> void deallocateDelete(Allocator& allocator, T& object)
		{
			object.~T();
			allocator.deallocate(&object);
		}

		template<class T> T* allocateArray(Allocator* allocator, const size_t& length)
		{
			assert(length != 0);

			unsigned __int8 headerSize = sizeof(size_t) / sizeof(T);

			if (sizeof(size_t) % sizeof(T) > 0)
				headerSize += 1;

			T* p = nullptr;

			//Allocate extra space to store array length in the bytes before the array
			p = ((T*)allocator->allocate(sizeof(T)*(length + headerSize), __alignof(T))) + headerSize;

			*(((size_t*)p) - 1) = length;

			for (size_t i = 0; i < length; i++)
				new (&p[i]) T;

			return p;
		}
		template<class T> T* allocateArray(Allocator* allocator, const size_t& length, T&& t)
		{
			assert(length != 0);

			unsigned __int8 headerSize = sizeof(size_t) / sizeof(T);

			if (sizeof(size_t) % sizeof(T) > 0)
				headerSize += 1;

			T* p = nullptr;

			//Allocate extra space to store array length in the bytes before the array
			p = ((T*)allocator->allocate(sizeof(T)*(length + headerSize), __alignof(T))) + headerSize;

			*(((size_t*)p) - 1) = length;

			for (size_t i = 0; i < length; i++)
			{
				new (&p[i]) T(t);
			}

			return p;
		}

		template<class T> void deallocateArray(Allocator* allocator, T* array)
		{
			assert(array != nullptr);

			size_t length = *(((size_t*)array) - 1);

			for (size_t i = 0; i < length; i++)
				array[i].~T();

			//Calculate how much extra memory was allocated to store the length before the array
			unsigned __int8 headerSize = sizeof(size_t) / sizeof(T);

			if (sizeof(size_t) % sizeof(T) > 0)
				headerSize += 1;

			allocator->deallocate(array - headerSize);
		}

		namespace pointer_math
		{
			inline void* alignForward(void* address, const unsigned __int8& alignment)
			{
				return (void*)((reinterpret_cast<uintptr_t>(address) + static_cast<uintptr_t>(alignment - 1)) & static_cast<uintptr_t>(~(alignment - 1)));
			}

			inline const void* alignForward(const void* address, const unsigned __int8& alignment)
			{
				return (void*)((reinterpret_cast<uintptr_t>(address) + static_cast<uintptr_t>(alignment - 1)) & static_cast<uintptr_t>(~(alignment - 1)));
			}

			inline void* alignBackward(void* address, const unsigned __int8& alignment)
			{
				return (void*)(reinterpret_cast<uintptr_t>(address) & static_cast<uintptr_t>(~(alignment - 1)));
			}

			inline const void* alignBackward(const void* address, const unsigned __int8& alignment)
			{
				return (void*)(reinterpret_cast<uintptr_t>(address) & static_cast<uintptr_t>(~(alignment - 1)));
			}

			inline unsigned __int8 alignForwardAdjustment(const void* address, const unsigned __int8& alignment)
			{
				unsigned __int8 adjustment = alignment - (reinterpret_cast<uintptr_t>(address) & static_cast<uintptr_t>(alignment - 1));

				if (adjustment == alignment)
					return 0; //already aligned

				return adjustment;
			}

			inline unsigned __int8 alignForwardAdjustmentWithHeader(const void* address, const unsigned __int8& alignment, const unsigned __int8& headerSize)
			{
				unsigned __int8 adjustment = alignForwardAdjustment(address, alignment);

				unsigned __int8 neededSpace = headerSize;

				if (adjustment < neededSpace)
				{
					neededSpace -= adjustment;

					//Increase adjustment to fit header
					adjustment += alignment * (neededSpace / alignment);

					if (neededSpace % alignment > 0)
						adjustment += alignment;
				}

				return adjustment;
			}

			inline unsigned __int8 alignBackwardAdjustment(const void* address, const unsigned __int8& alignment)
			{
				unsigned __int8 adjustment = reinterpret_cast<uintptr_t>(address) & static_cast<uintptr_t>(alignment - 1);

				if (adjustment == alignment)
					return 0; //already aligned

				return adjustment;
			}

			inline void* add(void* p, const size_t& x)
			{
				return (void*)(reinterpret_cast<uintptr_t>(p) + x);
			}

			inline const void* add(const void* p, const size_t& x)
			{
				return (const void*)(reinterpret_cast<uintptr_t>(p) + x);
			}

			inline void* subtract(void* p, const size_t& x)
			{
				return (void*)(reinterpret_cast<uintptr_t>(p) - x);
			}

			inline const void* subtract(const void* p, const size_t& x)
			{
				return (const void*)(reinterpret_cast<uintptr_t>(p) - x);
			}
		};
	}
}