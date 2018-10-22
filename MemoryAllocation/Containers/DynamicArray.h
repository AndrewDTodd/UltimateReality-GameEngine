#pragma once

#include <iostream>
#include "../Memory.h"

namespace Ultrality
{
	namespace Memory
	{
		template<typename Type>
		class DynamicArray
		{
		//private:
		public:
			Type* m_array;
			size_t m_arraySize;
			size_t m_initialSize;
			Allocator* m_allocator;
			Type m_TypeNULL = Type();
			size_t m_bufferSize;

			DynamicArray(const size_t& initialSize, const size_t& bufferSize, Allocator* p_allocator)
			{
				//this->m_array = new Type[initialSize + bufferSize];
				this->m_array = Memory::allocateArray<Type>(p_allocator, (initialSize + bufferSize));
				this->m_bufferSize = bufferSize;
				this->m_arraySize = initialSize + bufferSize;
				this->m_initialSize = initialSize;
				this->m_allocator = p_allocator;
			}
			~DynamicArray()
			{
				Memory::deallocateArray<Type>(this->m_allocator, this->m_array);
				//delete[] this->m_array;
				this->m_array = nullptr;
				this->m_allocator = nullptr;
			}

			Type& operator[](const int& index)
			{
				return this->m_array[index];
			}

			/*template<class Type>
			void DynamicArray<Type>::operator=(void* address, const Type& data)
			{
			if (address >= this->m_array && address <= (this->m_array + this->arraySize))
			{
			&address = data;
			}
			}*/

			void grow()
			{
				static Type* newArray = Memory::allocateArray<Type>(this->m_allocator, (this->m_arraySize + this->m_bufferSize));
				//static void* newArray = new Type[this->m_arraySize + this->m_bufferSize];

				for (int i = 0; i < this->m_arraySize; i++)
				{
					if (this->m_array[i] == this->m_TypeNULL && i != 0)
					{
						Memory::deallocateArray<Type>(this->m_allocator, this->m_array);
						//delete[] this->m_array;
						this->m_array = newArray;
						this->m_arraySize = this->m_arraySize + this->m_bufferSize;
						newArray = nullptr;
						break;
					}

					else
					{
						newArray[i] = this->m_array[i];
					}
				}
			}

			size_t getNumOfFreeIndexs()
			{
				for (int i = 0; i < this->m_arraySize; i++)
				{
					if (this->m_array[i] == Type::null && i != 0)
					{
						return (this->m_arraySize - i);
					}
				}
			}

			void clear()
			{
				static Type* newArray = Memory::allocateArray<Type>(this->m_allocator, this->m_initialSize);
				//static void* newArray = new Type[this->m_initialSize];
				Memory::deallocateArray<Type>(this->m_allocator, this->m_array);
				//delete[] this->m_array;
				this->m_array = newArray;
				newArray = nullptr;

				this->m_arraySize = this->m_initialSize;
			}

			friend std::ostream& operator<<(std::ostream& stream, const DynamicArray& dynamicArray)
			{
				for (int i = 0; i < dynamicArray.m_arraySize; i++)
				{
					/*if (dynamicArray.m_array[i] == )
					{
						stream << "[" << i << "]" << ": NULL ,";
					}*/
					//else
						stream << "[" << i << "]" << ":" << dynamicArray.m_array[i] << ",";
				}

				return stream;
			}
		};
	}
}