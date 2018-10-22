#pragma once

#include <iostream>
#include "../Allocator.h"
#include "../PoolAllocator.h"
#include <assert.h>

namespace Ultrality
{
	namespace Memory
	{
		template<typename Type>
		class Node
		{
			template<typename Type>
			friend class LinkedList;
		public:
			Node<Type>* m_Prev = nullptr;
			Node<Type>* m_Next = nullptr;
		public:
			Type* m_Element = nullptr;
			Node()
			{
				this->m_Next = nullptr;
				this->m_Prev = nullptr;
				this->m_Element = nullptr;
			}
			~Node()
			{
				this->m_Next = nullptr;
				this->m_Prev = nullptr;
				this->m_Element = nullptr;
			}
		};

		template<typename Type>
		class LinkedList
		{
		private:
			Allocator* m_parentAllocator;
		public:
			PoolAllocator* m_Allocator;
			PoolAllocator* m_TypeAllocator;

			Node<Type> m_root;
			
			size_t m_listLength = 0;

			LinkedList(const size_t& allocatorSize, void* p)
			{
				//this->m_root.m_Prev = this->m_root.m_Next = this->m_root.m_Element = nullptr;
				this->m_Allocator = new (p) PoolAllocator(sizeof(Node<Type>), alignof(Node<Type>), allocatorSize, pointer_math::add(p, sizeof(PoolAllocator)));
				this->m_TypeAllocator = nullptr;

				this->m_parentAllocator = nullptr;
			}
			LinkedList(const size_t& allocatorSize, Allocator* allocator)
			{
				//this->m_root.m_Prev = this->m_root.m_Next = this->m_root.m_Element = nullptr;
				void* p = allocator->allocate(allocatorSize + sizeof(PoolAllocator), __alignof(PoolAllocator));
				this->m_Allocator = new (p) PoolAllocator(sizeof(Node<Type>), alignof(Node<Type>), allocatorSize, pointer_math::add(p, sizeof(PoolAllocator)));
				this->m_TypeAllocator = nullptr;

				this->m_parentAllocator = allocator;
			}
			LinkedList(const size_t& allocatorSize, const size_t& typeAllocatorSize, Allocator* allocator)
			{
				//this->m_root.m_Prev = this->m_root.m_Next = this->m_root.m_Element = nullptr;
				void* p = allocator->allocate(allocatorSize + sizeof(PoolAllocator), __alignof(PoolAllocator));
				this->m_Allocator = new (p) PoolAllocator(sizeof(Node<Type>), alignof(Node<Type>), allocatorSize, pointer_math::add(p, sizeof(PoolAllocator)));

				if (typeAllocatorSize != 0)
				{
					void* p = allocator->allocate(typeAllocatorSize + sizeof(PoolAllocator), __alignof(PoolAllocator));
					this->m_TypeAllocator = new (p) PoolAllocator(sizeof(Type), alignof(Type), typeAllocatorSize, pointer_math::add(p, sizeof(PoolAllocator)));
				}
				else
				{
					this->m_TypeAllocator = nullptr;
				}

				this->m_parentAllocator = allocator;
			}
			LinkedList(const size_t& allocatorSize, Allocator* allocator, Type* elements, const size_t& arraySize)
			{
				//this->m_root.m_Prev = this->m_root.m_Next = this->m_root.m_Element = nullptr;
				void* p = allocator->allocate(allocatorSize + sizeof(PoolAllocator), __alignof(PoolAllocator));
				this->m_Allocator = new (p) PoolAllocator(sizeof(Node<Type>), alignof(Node<Type>), allocatorSize, pointer_math::add(p, sizeof(PoolAllocator)));
				this->m_TypeAllocator = nullptr;

				for (int i = 0; i < arraySize; i++)
				{
					if (this->m_root.m_Next == nullptr)
					{
						this->AddFirst(&elements[i]);
					}

					else
					{
						this->PrependNode(&elements[i]);
					}
				}

				this->m_parentAllocator = allocator;
			}
			LinkedList(LinkedList<Type>&& list)
			{
				void* p = list.m_parentAllocator->allocate(list.m_Allocator->m_size + sizeof(PoolAllocator), __alignof(PoolAllocator));
				this->m_Allocator = new (p) PoolAllocator(sizeof(Node<Type>), alignof(Node<Type>), list.m_Allocator->m_size, pointer_math::add(p, sizeof(PoolAllocator)));

				if (list.m_TypeAllocator)
				{
					void* tp = list.m_parentAllocator->allocate(list.m_TypeAllocator->m_size + sizeof(PoolAllocator), __alignof(PoolAllocator));
					this->m_TypeAllocator = new (tp) PoolAllocator(sizeof(Type), alignof(Type), list.m_TypeAllocator->m_size, pointer_math::add(tp, sizeof(PoolAllocator)));
				}
				else
				{
					this->m_TypeAllocator = nullptr;
				}

				this->m_listLength = 0;
				this->m_parentAllocator = list.m_parentAllocator;
			}
			LinkedList(const LinkedList<Type>& list)
			{
				void* p = list.m_parentAllocator->allocate(list.m_Allocator->m_size + sizeof(PoolAllocator), __alignof(PoolAllocator));
				this->m_Allocator = new (p) PoolAllocator(sizeof(Node<Type>), alignof(Node<Type>), list.m_Allocator->m_size, pointer_math::add(p, sizeof(PoolAllocator)));

				if (list.m_TypeAllocator)
				{
					void* tp = list.m_parentAllocator->allocate(list.m_TypeAllocator->m_size + sizeof(PoolAllocator), __alignof(PoolAllocator));
					this->m_TypeAllocator = new (tp) PoolAllocator(sizeof(Type), alignof(Type), list.m_TypeAllocator->m_size, pointer_math::add(tp, sizeof(PoolAllocator)));
				}
				else
				{
					this->m_TypeAllocator = nullptr;
				}

				this->m_listLength = 0;
				this->m_parentAllocator = list.m_parentAllocator;
			}

			~LinkedList()
			{
				if (this->m_TypeAllocator == nullptr)
				{
					if (this->m_listLength != 0)
					{
						this->emptyList();
					}
					if(this->m_parentAllocator)
						Memory::deallocateDelete<PoolAllocator>(*this->m_parentAllocator, *this->m_Allocator);
					this->m_Allocator = nullptr;
				}
				else
				{
					if (this->m_listLength != 0)
					{
						this->emptyListDeleteDynam();
					}
					Memory::deallocateDelete<PoolAllocator>(*this->m_parentAllocator, *this->m_Allocator);
					this->m_Allocator = nullptr;
					Memory::deallocateDelete<PoolAllocator>(*this->m_parentAllocator, *this->m_TypeAllocator);
					this->m_TypeAllocator = nullptr;
				}
			}

			void AddFirst(Type* element)
			{
#ifdef DEBUG
				assert(this->m_TypeAllocator == nullptr);
#endif // DEBUG
				static Node<Type>* newNode;
				newNode = (Node<Type>*) this->m_Allocator->allocate(sizeof(Node<Type>), alignof(Node<Type>));
				newNode->m_Element = element;
				newNode->m_Next = nullptr;
				newNode->m_Prev = nullptr;
				this->m_root.m_Next = newNode;
				this->m_root.m_Prev = newNode;
				newNode = nullptr;
				this->m_listLength++;
			}
			void AddFirstDynam(Type&& element)
			{
#ifdef DEBUG
				assert(this->m_TypeAllocator);
#endif // DEBUG
				static Node<Type>* newNode;
				newNode = (Node<Type>*) this->m_Allocator->allocate(sizeof(Node<Type>), alignof(Node<Type>));
				this->m_root.m_Next = newNode;
				this->m_root.m_Prev = newNode;
				newNode->m_Element = Memory::allocateNew<Type>(sizeof(Type), *this->m_TypeAllocator, element);
				newNode = nullptr;
				this->m_listLength++;
			}

			void PrependNode(Type* element)
			{
#ifdef DEBUG
				assert(this->m_TypeAllocator == nullptr);
#endif // DEBUG
				static Node<Type>* newNode;
				newNode = (Node<Type>*) this->m_Allocator->allocate(sizeof(Node<Type>), alignof(Node<Type>));
				newNode->m_Element = element;
				newNode->m_Next = this->m_root.m_Next;
				newNode->m_Prev = &this->m_root;
				this->m_root.m_Next->m_Prev = newNode;
				this->m_root.m_Next = newNode;
				newNode = nullptr;
				this->m_listLength++;
			}
			void PrependNodeDynam(Type&& element)
			{
#ifdef DEBUG
				assert(this->m_TypeAllocator);
#endif // DEBUG
				static Node<Type>* newNode;
				newNode = (Node<Type>*) this->m_Allocator->allocate(sizeof(Node<Type>), alignof(Node<Type>));
				newNode->m_Next = this->m_root.m_Next;
				newNode->m_Prev = &this->m_root;
				this->m_root.m_Next->m_Prev = newNode;
				this->m_root.m_Next = newNode;
				newNode->m_Element = Memory::allocateNew<Type>(sizeof(Type), *this->m_TypeAllocator, element);
				newNode = nullptr;
				this->m_listLength++;
			}

			void AppendNode(Type* element)
			{
#ifdef DEBUG
				assert(this->m_TypeAllocator == nullptr);
#endif // DEBUG
				static Node<Type>* newNode;
				newNode = (Node<Type>*) this->m_Allocator->allocate(sizeof(Node<Type>), alignof(Node<Type>));
				newNode->m_Element = element;
				newNode->m_Prev = this->m_root.m_Prev;
				newNode->m_Next = &this->m_root;
				this->m_root.m_Prev->m_Next = newNode;
				this->m_root.m_Prev = newNode;
				newNode = nullptr;
				this->m_listLength++;
			}
			void AppendNodeDynam(Type&& element)
			{
#ifdef DEBUG
				assert(this->m_TypeAllocator);
#endif // DEBUG
				static Node<Type>* newNode;
				newNode = (Node<Type>*) this->m_Allocator->allocate(sizeof(Node<Type>), alignof(Node<Type>));
				newNode->m_Prev = this->m_root.m_Prev;
				newNode->m_Next = &this->m_root;
				this->m_root.m_Prev->m_Next = newNode;
				this->m_root.m_Prev = newNode;
				newNode->m_Element = Memory::allocateNew<Type>(sizeof(Type), *this->m_TypeAllocator, element);
				newNode = nullptr;
				this->m_listLength++;
			}

			void InsertNode(Type* element, const size_t& index)
			{
#ifdef DEBUG
				assert(index >= 0 && index <= this->m_listLength && this->m_TypeAllocator == nullptr);
#endif // DEBUG
				static Node<Type>* newNode;
				newNode = (Node<Type>*) this->m_Allocator->allocate(sizeof(Node<Type>), alignof(Node<Type>));
				if (this->m_root.m_Next == nullptr)
				{
					newNode->m_Element = element;
					this->m_root.m_Next = newNode;
					this->m_root.m_Prev = newNode;
					newNode->m_Prev = &this->m_root;
					newNode->m_Next = &this->m_root;
					newNode = nullptr;
					this->m_listLength++;
					return;
				}

				else
				{
					size_t count = 0;
					Node<Type>* prev = &this->m_root;
					Node<Type>* current = &this->m_root;
					while (current)
					{
						if (count == index)
						{
							newNode->m_Element = element;
							prev->m_Next = newNode;
							current->m_Prev = newNode;
							newNode->m_Next = current;
							newNode->m_Prev = prev;
							newNode = nullptr;
							prev = nullptr;
							current = nullptr;
							this->m_listLength++;
							return;
						}

						if (index <= this->m_listLength / 2)
						{
							prev = current;
							current = prev->m_Next;
						}
						else
						{
							prev = current;
							current = prev->m_Prev;
						}
						count++;
					}
				}
			}
			void InsertNodeDynam(Type&& element, const size_t& index)
			{
#ifdef DEBUG
				assert(index >= 0 && index <= this->m_listLength && this->m_TypeAllocator);
#endif // DEBUG
				static Node<Type>* newNode;
				newNode = (Node<Type>*) this->m_Allocator->allocate(sizeof(Node<Type>), alignof(Node<Type>));
				if (this->m_root.m_Next == nullptr)
				{
					this->m_root.m_Next = newNode;
					this->m_root.m_Prev = newNode;
					newNode->m_Prev = this->m_root;
					newNode->m_Next = this->m_root;
					newNode->m_Element = Memory::allocateNew<Type>(sizeof(Type), *this->m_TypeAllocator, element);
					newNode = nullptr;
					prev = nullptr;
					current = nullptr;
					this->m_listLength++;
					return;
				}

				else
				{
					size_t count = 0;
					Node<Type>* prev = this->m_root;
					Node<Type>* current = this->m_root;
					while (current)
					{
						if (count == index)
						{
							prev->m_Next = newNode;
							current->m_Prev = newNode;
							newNode->m_Next = current;
							newNode->m_Prev = prev;
							newNode->m_Element = Memory::allocateNew<Type>(sizeof(Type), *this->m_TypeAllocator, element);
							newNode = nullptr;
							prev = nullptr;
							current = nullptr;
							this->m_listLength++;
							return;
						}

						if (index <= this->m_listLength / 2)
						{
							prev = current;
							current = prev->m_Next;
						}
						else
						{
							prev = current;
							current = prev->m_Prev;
						}
						count++;
					}
				}
			}

			void removeFirst()
			{
#ifdef DEBUG
				assert(this->m_TypeAllocator == nullptr);
#endif // DEBUG
				Node<Type>* node = this->m_root.m_Next;
				if (node->m_Next)
				{
					this->m_root.m_Next = node->m_Next;
					this->m_root.m_Next->m_Prev = &this->m_root;
				}
				else
				{
					this->m_root.m_Next = nullptr;
					this->m_root.m_Prev = nullptr;
				}
				node->m_Next = nullptr;
				node->m_Prev = nullptr;
				node->m_Element = nullptr;
				Memory::deallocateDelete<Node<Type>>(*this->m_Allocator, *node);
				node = nullptr;
				this->m_listLength--;
			}
			void removeFirstDeleteDynam()
			{
#ifdef DEBUG
				assert(this->m_TypeAllocator);
#endif // DEBUG
				Node<Type>* node = this->m_root.m_Next;
				if (node->m_Next)
				{
					this->m_root.m_Next = node->m_Next;
					this->m_root.m_Next->m_Prev = &this->m_root;
				}
				else
				{
					this->m_root.m_Next = nullptr;
				}
				node->m_Next = nullptr;
				node->m_Prev = nullptr;
				Memory::deallocateDelete<Type>(*this->m_TypeAllocator, *node->m_Element);
				node->m_Element = nullptr;
				Memory::deallocateDelete<Node<Type>>(*this->m_Allocator, *node);
				node = nullptr;
				this->m_listLength--;
			}

			void removeLast()
			{
#ifdef DEBUG
				assert(this->m_TypeAllocator == nullptr);
#endif // DEBUG
				Node<Type>* node = this->m_root.m_Prev;
				this->m_root.m_Prev = node->m_Prev;
				this->m_root.m_Prev->m_Next = this->m_root;
				node->m_Next = nullptr;
				node->m_Prev = nullptr;
				node->m_Element = nullptr;
				Memory::deallocateDelete<Node<Type>>(*this->m_Allocator, *node);
				node = nullptr;
				this->m_listLength--;
			}
			void removeLastDeleteDynam()
			{
#ifdef DEBUG
				assert(this->m_TypeAllocator);
#endif // DEBUG
				Node<Type>* node = this->m_root.m_Prev;
				this->m_root.m_Prev = node->m_Prev;
				this->m_root.m_Prev->m_Next = this->m_root;
				node->m_Next = nullptr;
				node->m_Prev = nullptr;
				Memory::deallocateDelete<Type>(*this->m_TypeAllocator, *node->m_Element);
				node->m_Element = nullptr;
				Memory::deallocateDelete<Node<Type>>(*this->m_Allocator, *node);
				node = nullptr;
				this->m_listLength--;
			}

			void removeNode(const size_t& index)
			{
#ifdef DEBUG
				assert(this->m_listLength != 0 && index >= 0 && index <= this->m_listLength - 1 && this->m_TypeAllocator == nullptr);
#endif // DEBUG
				size_t count = 0;
				Node<Type>* prev = this->m_root;
				Node<Type>* current = this->m_root;
				while (current)
				{
					if (count == index)
					{
						if (index <= this->m_listLength / 2)
						{
							prev->m_Next = current->m_Next;
							current->m_Next->m_Prev = prev;
							current->m_Next = nullptr;
							current->m_Prev = nullptr;
							current->m_Element = nullptr;
							Memory::deallocateDelete<Node<Type>>(*this->m_Allocator, *current);
							current = nullptr;
							prev = nullptr;
							this->m_listLength--;
							return;
						}

						else
						{
							prev->m_Prev = current->m_Prev;
							current->m_Prev->m_Next = prev;
							current->m_Next = nullptr;
							current->m_Prev = nullptr;
							current->m_Element = nullptr;
							Memory::deallocateDelete<Node<Type>>(*this->m_Allocator, *current);
							current = nullptr;
							prev = nullptr;
							this->m_listLength--;
							return;
						}
					}

					if (index <= this->m_listLength / 2)
					{
						prev = current;
						current = prev->m_Next;
					}
					else
					{
						prev = current;
						current = prev->m_Prev;
					}
					count++;
				}
			}
			void removeNodeDeleteDynam(const size_t& index)
			{
#ifdef DEBUG
				assert(this->m_listLength != 0 && index >= 0 && index <= this->m_listLength - 1 && this->m_TypeAllocator);
#endif // DEBUG
				size_t count = 0;
				Node<Type>* prev = this->m_root;
				Node<Type>* current = this->m_root;
				while (current)
				{
					if (count == index)
					{
						if (index <= this->m_listLength / 2)
						{
							prev->m_Next = current->m_Next;
							current->m_Next->m_Prev = prev;
							current->m_Next = nullptr;
							current->m_Prev = nullptr;
							Memory::deallocateDelete<Type>(*this->m_TypeAllocator, *node->m_Element);
							current->m_Element = nullptr;
							Memory::deallocateDelete<Node<Type>>(*this->m_Allocator, *current);
							current = nullptr;
							prev = nullptr;
							this->m_listLength--;
							return;
						}

						else
						{
							prev->m_Prev = current->m_Prev;
							current->m_Prev->m_Next = prev;
							current->m_Next = nullptr;
							current->m_Prev = nullptr;
							Memory::deallocateDelete<Type>(*this->m_TypeAllocator, *node->m_Element);
							current->m_Element = nullptr;
							Memory::deallocateDelete<Node<Type>>(*this->m_Allocator, *current);
							current = nullptr;
							prev = nullptr;
							this->m_listLength--;
							return;
						}
					}

					if (index <= this->m_listLength / 2)
					{
						prev = current;
						current = prev->m_Next;
					}
					else
					{
						prev = current;
						current = prev->m_Prev;
					}
					count++;
				}
			}

			void emptyList()
			{
#ifdef DEBUG
				assert(this->m_TypeAllocator == nullptr);
#endif // DEBUG
				if (this->m_listLength == 0)
				{
					return;
				}
				Node<Type>* currentNode = this->m_root.m_Prev;
				for (int i = 0; i < this->m_listLength; i++)
				{
					this->m_root.m_Prev = currentNode->m_Prev;
					Memory::deallocateDelete<Node<Type>>(*this->m_Allocator, *currentNode);
					currentNode = this->m_root.m_Prev;
				}

				this->m_listLength = 0;
				this->m_root.m_Next = nullptr;
				this->m_root.m_Prev = nullptr;
			}
			void emptyListDeleteDynam()
			{
#ifdef DEBUG
				assert(this->m_TypeAllocator);
#endif // DEBUG
				if (this->m_listLength == 0)
				{
					return;
				}
				Node<Type>* currentNode = this->m_root.m_Prev;
				while(currentNode != &this->m_root)
				{
					this->m_root.m_Prev = currentNode->m_Prev;
					if (currentNode->m_Element)
					{
						Memory::deallocateDelete<Type>(*this->m_TypeAllocator, *currentNode->m_Element);
						this->m_listLength--;
					}
					Memory::deallocateDelete<Node<Type>>(*this->m_Allocator, *currentNode);
					currentNode = this->m_root.m_Prev;
				}

#ifdef DEBUG
				assert(this->m_listLength == 0);
#endif // DEBUG

				this->m_root.m_Next = nullptr;
				this->m_root.m_Prev = nullptr;
			}

			template<typename Type>
			friend std::ostream& operator<<(std::ostream& stream, const LinkedList<Type>& list)
			{
				if (list.m_listLength == 0)
				{
					stream << "List empty" << "\n";
					return stream;
				}
				Node<Type>* current = list.m_root.m_Next;
				Node<Type>* prev = list.m_root.m_Next;
				for (int i = 0; i < list.m_listLength; i++)
				{
					stream << "Node" << "[" << i << "]" << ":"  << typeid(Type).name() << "\n" << *current->m_Element << "\n";

					prev = current;
					current = prev->m_Next;
				}

				stream << "\n";
				return stream;
			}
		};
	}
}