#pragma once

#include "LinkedList.h"
#include "../PoolAllocator.h"
#include <iostream>
#include <assert.h>
#include "../../GlobalUtilities/GlobalUtilities.h"

namespace Ultrality
{
	namespace Memory
	{
		template<typename Key, typename Value>
		class HashNode
		{
			template<typename Key, typename Value>
			friend class OpenHashTable;
		public:
			unsigned __int32 m_Hash;
			Value* m_Value;

#ifdef DEBUG
			Key* m_Key;
#endif // DEBUG


		public:
			HashNode(Key&& key, Value* value)
			{
				this->m_Hash = GlobUtil::GlobalUtilities::Hasher(key);
				this->m_Value = value;

#ifdef DEBUG
				this->m_Key;
#endif // DEBUG

			}
			HashNode()
			{
				this->m_Hash = 0;
				this->m_Value = nullptr;

#ifdef DEBUG
				this->m_Key = nullptr;
#endif // DEBUG

			}
			~HashNode()
			{
				this->m_Value = nullptr;
#ifdef DEBUG
				this->m_Key = nullptr;
#endif // DEBUG

			}

			template<typename Key, typename Value>
			friend std::ostream& operator<<(std::ostream& stream, const HashNode<Key, Value>& node)
			{
#ifdef DEBUG
				stream << "Key: " << *node.m_Key << "\n";
#endif // DEBUG
				stream << "HashKey: " << node.m_Hash << "\n" << *node.m_Value << "\n";

				return stream;
			}
		};
		

		//A Dictionary of values sorted by their associated 32 bit int hash values
		//collisions are handled by pushing back stationary object into the linkedList that makes up the hit row
		//hashes can be generated by any String or 32bit data Type (Key must = string || int, float)
		//must dynamicaly allocate the HashNodes, can dynamicaly allocate Values
		template<typename Key, typename Value>
		class OpenHashTable
		{
		private:
			Allocator* m_parentAllocator;
		public:
			LinkedList<HashNode<Key, Value>>* m_Dictionary;
			PoolAllocator* m_NodeAllocator;
			PoolAllocator* m_ValueAllocator;
			unsigned __int32 m_tableSize = 0;
			unsigned __int32 m_usedTableSpace = 0;
			Value** m_dynamAllocValues;
			unsigned __int32 m_dynamAllocValuesRegistered = 0;
			HashNode<Key, Value>** m_dynamAllocNodes;
			unsigned __int32 m_dynamAllocNodesRegistered = 0;

			//Creates a Dictionary of "arraySize" rows of LinkedLists up to "listsDepths" long
			//does not allow dynamic allocation of "Values"
			OpenHashTable(const unsigned __int32& arraySize, const unsigned __int32& listsDepths, Allocator* allocator)
			{
				size_t listsAllocatorsSizes = (sizeof(Node<HashNode<Key, Value>>) * listsDepths);
				size_t nodeAllocatorSize = (sizeof(HashNode<Key, Value>) * (arraySize * listsDepths));

				this->m_Dictionary = Memory::allocateArray<LinkedList<HashNode<Key, Value>>>(allocator, arraySize, LinkedList<HashNode<Key, Value>>(listsAllocatorsSizes, allocator));
				
				void* p = allocator->allocate(nodeAllocatorSize + sizeof(PoolAllocator), __alignof(PoolAllocator));
				this->m_NodeAllocator = new (p) PoolAllocator(sizeof(HashNode<Key, Value>), alignof(HashNode<Key, Value>), nodeAllocatorSize, pointer_math::add(p, sizeof(PoolAllocator)));

				this->m_dynamAllocNodes = Memory::allocateArray<HashNode<Key, Value>*>(allocator, (arraySize * (listsAllocatorsSizes / sizeof(Node<HashNode<Key, Value>>))));

				this->m_ValueAllocator = nullptr;
				this->m_dynamAllocValues = nullptr;
				this->m_tableSize = arraySize;

				this->m_parentAllocator = allocator;
			}

			//Creates a Dictionary of "arraySize" rows of LinkedLists up to "listsDepths" long
			//will allow dynamic allocation of "Values"
			OpenHashTable(Allocator* allocator, const unsigned __int32& arraySize, const unsigned __int32& listsDepths)// const unsigned __int32& valueAllocatorSize = (sizeof(Value) * (arraySize * listsDepths)))
			{
				size_t listsAllocatorsSizes = (sizeof(Node<HashNode<Key, Value>>) * listsDepths);
				size_t nodeAllocatorSize = (sizeof(HashNode<Key, Value>) * (arraySize * listsDepths));
				size_t valueAllocatorSize = (sizeof(Value) * (arraySize * listsDepths));

				this->m_Dictionary = Memory::allocateArray<LinkedList<HashNode<Key, Value>>>(allocator, arraySize, LinkedList<HashNode<Key, Value>>(listsAllocatorsSizes, allocator));
				
				void* p = allocator->allocate(nodeAllocatorSize + sizeof(PoolAllocator), __alignof(PoolAllocator));
				this->m_NodeAllocator = new (p) PoolAllocator(sizeof(HashNode<Key, Value>), alignof(HashNode<Key, Value>), nodeAllocatorSize, pointer_math::add(p, sizeof(PoolAllocator)));

				this->m_dynamAllocNodes = Memory::allocateArray<HashNode<Key, Value>*>(allocator, (arraySize * (listsAllocatorsSizes / sizeof(Node<HashNode<Key, Value>>))));

				void* vp = allocator->allocate(valueAllocatorSize + sizeof(PoolAllocator), __alignof(PoolAllocator));
				this->m_ValueAllocator = new (vp) PoolAllocator(sizeof(Value), alignof(Value), valueAllocatorSize, pointer_math::add(vp, sizeof(PoolAllocator)));

				this->m_tableSize = arraySize;

				this->m_dynamAllocValues = Memory::allocateArray<Value*>(allocator, (arraySize * (listsAllocatorsSizes / sizeof(Node<HashNode<Key, Value>>))));

				this->m_parentAllocator = allocator;
			}

			~OpenHashTable()
			{
#ifdef DEBUG
				assert(this->m_parentAllocator);
#endif // DEBUG
				this->ClearDynamAllocs();

				Memory::deallocateArray<LinkedList<HashNode<Key, Value>>>(this->m_parentAllocator, this->m_Dictionary);
				this->m_Dictionary = nullptr;

				if (this->m_ValueAllocator)
				{
					Memory::deallocateDelete<PoolAllocator>(*this->m_parentAllocator, *this->m_NodeAllocator);
					this->m_NodeAllocator = nullptr;
					Memory::deallocateArray<HashNode<Key, Value>*>(this->m_parentAllocator, this->m_dynamAllocNodes);
					this->m_dynamAllocNodes = nullptr;
					Memory::deallocateDelete<PoolAllocator>(*this->m_parentAllocator, *this->m_ValueAllocator);
					this->m_ValueAllocator = nullptr;
					Memory::deallocateArray<Value*>(this->m_parentAllocator, this->m_dynamAllocValues);
					this->m_dynamAllocValues = nullptr;
				}
				else
				{
#ifdef DEBUG
					assert(!this->m_ValueAllocator);
#endif // DEBUG

					Memory::deallocateDelete<PoolAllocator>(*this->m_parentAllocator, *this->m_NodeAllocator);
					this->m_NodeAllocator = nullptr;
					Memory::deallocateArray<HashNode<Key, Value>*>(this->m_parentAllocator, this->m_dynamAllocNodes);
					this->m_dynamAllocNodes = nullptr;
				}
			}

			//inserts a new Node, must pass in pointer to a "Value" object
			//key must be a 32bit int, float or a string
			void Insert(Key&& key, Value* value)
			{
#ifdef DEBUG
				assert(this->m_NodeAllocator && !this->m_ValueAllocator);
#endif // DEBUG
				if (this->m_usedTableSpace == this->m_tableSize)
				{
#ifdef DEBUG
					std::cout << "Table is full" << std::endl;
#endif // DEBUG
					return;
				}
				static HashNode<Key, Value>* newNode = (HashNode<Key, Value>*) this->m_NodeAllocator->allocate(sizeof(HashNode<Key, Value>), alignof(HashNode<Key, Value>));
				this->m_dynamAllocNodes[this->m_dynamAllocNodesRegistered] = newNode;
				this->m_dynamAllocNodesRegistered++;

				newNode->m_Hash = GlobUtil::GlobalUtilities::Hasher(key);
				newNode->m_Value = value;
#ifdef DEBUG
				newNode->m_Key = Memory::allocateNew<Key>(sizeof(Key), *this->m_parentAllocator, key);
#endif // DEBUG

				if (this->m_Dictionary[(newNode->m_Hash % this->m_tableSize)].m_listLength == 0)
				{
					this->m_usedTableSpace++;
				}

				if (this->m_Dictionary[(newNode->m_Hash % this->m_tableSize)].m_listLength == 0)
				{
					this->m_Dictionary[(newNode->m_Hash % this->m_tableSize)].AddFirst(newNode);
				}
				else
				{
					this->m_Dictionary[(newNode->m_Hash % this->m_tableSize)].PrependNode(newNode);
				}
				newNode = nullptr;
			}

			//inserts a Node and dynamAllocates the value
			//key must be a 32bit int, float, or string
			void InsertDynamic(Key&& key, Value&& value)
			{
#ifdef DEBUG
				assert(this->m_ValueAllocator);
#endif // DEBUG
				if (this->m_usedTableSpace == this->m_tableSize)
				{
#ifdef DEBUG
					std::cout << "Table is full" << std::endl;
#endif // DEBUG
					return;
				}
				static HashNode<Key, Value>* newNode = (HashNode<Key, Value>*) this->m_NodeAllocator->allocate(sizeof(HashNode<Key, Value>), alignof(HashNode<Key, Value>));
				this->m_dynamAllocNodes[this->m_dynamAllocNodesRegistered] = newNode;
				this->m_dynamAllocNodesRegistered++;

				newNode->m_Hash = GlobUtil::GlobalUtilities::Hasher(key);
				newNode->m_Value = Memory::allocateNew<Value>(sizeof(Value), *this->m_ValueAllocator, value);
				this->m_dynamAllocValues[this->m_dynamAllocValuesRegistered] = newNode->m_Value;
				this->m_dynamAllocValuesRegistered++;

#ifdef DEBUG
				newNode->m_Key = Memory::allocateNew<Key>(sizeof(Key), *this->m_parentAllocator, key);
#endif // DEBUG

				if (this->m_Dictionary[(newNode->m_Hash % this->m_tableSize)].m_listLength == 0)
				{
					this->m_usedTableSpace++;
				}

				if (this->m_Dictionary[(newNode->m_Hash % this->m_tableSize)].m_listLength == 0)
				{
					this->m_Dictionary[(newNode->m_Hash % this->m_tableSize)].AddFirst(newNode);
				}
				else
				{
					this->m_Dictionary[(newNode->m_Hash % this->m_tableSize)].PrependNode(newNode);
				}
				newNode = nullptr;
			}

			void RemoveNode(Key& key)
			{
#ifdef DEBUG
				assert(this->m_usedTableSpace != 0);
#endif // DEBUG

				unsigned __int32 hash = GlobUtil::GlobalUtilities::Hasher(key);
				unsigned __int32 index = hash % this->m_tableSize;
				unsigned __int32 listIndex = 0;

				Node<HashNode<Key, Value>>* currentNode = this->m_Dictionary[index]->m_root;

				while (currentNode->m_Next->m_Element->m_Hash != hash)
				{
					if (currentNode->m_Next)
					{
						currentNode = currentNode->m_Next;
						listIndex++;
					}
					else
					{
#ifdef DEBUG
						std::cout << "No such key is held in this table" << std::endl;
#endif // DEBUG
						return;
					}
				}

#ifdef DEBUG
				assert(currentNode->m_Element->m_Hash == hash);
#endif // DEBUG

				this->m_Dictionary[index]->removeNode(listIndex);

#ifdef DEBUG
				Memory::deallocateDelete<Key>(*this->m_parentAllocator, *currentNode->m_Element->m_Key)
#endif // DEBUG

				Memory::deallocateDelete<HashNode<Key, Value>>(this->m_NodeAllocator, *currentNode->m_Element);
				if (listIndex == 0)
				{
					this->m_usedTableSpace--;
				}
			}
			void RemoveNodeDynam(Key& key)
			{
#ifdef DEBUG
				assert(this->m_usedTableSpace != 0);
#endif // DEBUG

				unsigned __int32 hash = GlobUtil::GlobalUtilities::Hasher(key);
				unsigned __int32 index = hash % this->m_tableSize;
				unsigned __int32 listIndex = 0;

				Node<HashNode<Key, Value>>* currentNode = this->m_Dictionary[index]->m_root;

				while (currentNode->m_Next->m_Element->m_Hash != hash)
				{
					if (currentNode->m_Next)
					{
						currentNode = currentNode->m_Next;
						listIndex++;
					}
					else
					{
#ifdef DEBUG
						std::cout << "No such key is held in this table" << std::endl;
#endif // DEBUG
						return;
					}
				}

#ifdef DEBUG
				assert(currentNode->m_Element->m_Hash == hash);
#endif // DEBUG

				this->m_Dictionary[index]->removeNode(listIndex);

				unsigned __int32 dynamValueIndex = 0;

				for (int i = 0; i < this->m_dynamAllocValuesRegistered; i++)
				{
					if (this->m_dynamAllocValues[dynamValueIndex] == currentNode->m_Element->m_Value)
					{
						Memory::deallocateDelete<Value>(*this->m_ValueAllocator, m_dynamAllocValues[dynamValueIndex]);
						this->m_dynamAllocValuesRegistered--;
					}
					else
					{
						dynamValueIndex++;
					}
				}

#ifdef DEBUG
				Memory::deallocateDelete<Key>(*this->m_parentAllocator, *currentNode->m_Element->m_Key)
#endif // DEBUG

				Memory::deallocateDelete<HashNode<Key, Value>>(*this->m_NodeAllocator, *currentNode->m_Element);

				if (listIndex == 0)
				{
					this->m_usedTableSpace--;
				}
			}

			HashNode<Key, Value>* Search(const Key& key)
			{
#ifdef DEBUG
				assert(this->m_usedTableSpace != 0);
#endif // DEBUG

				unsigned __int32 hash = GlobUtil::GlobalUtilities::Hasher(key);
				unsigned __int32 index = hash % this->m_tableSize;

				Node<HashNode<Key, Value>>* currentNode = this->m_Dictionary[index]->m_root;

				while (currentNode->m_Next->m_Element->m_Hash != hash)
				{
					if (currentNode->m_Next)
					{
						currentNode = currentNode->m_Next;
					}
					else
					{
#ifdef DEBUG
						std::cout << "No such key is held in this table" << std::endl;
#endif // DEBUG
						return;
					}
				}

				return currentNode;
			}

			void EmptyIndex(const int& index)
			{
				Node<HashNode<Key, Value>>* currentNode = this->m_Dictionary[index]->m_root->m_Prev;

				while (currentNode != this->m_Dictionary[index]->m_root)
				{
					this->m_Dictionary[index]->removeLast();

#ifdef DEBUG
					Memory::deallocateDelete<Key>(*this->m_parentAllocator, *currentNode->m_Element->m_Key)
#endif // DEBUG

					Memory::deallocateDelete<HashNode<Key, Value>>(*this->m_NodeAllocator, *currentNode->m_Element);

					currentNode = currentNode->m_Prev;
				}
#ifdef DEBUG
				assert(currentNode == this->m_Dictionary[index]->m_root);
#endif // DEBUG

				this->m_usedTableSpace--;
			}
			void EmptyIndexDynam(const int& index)
			{
				Node<HashNode<Key, Value>>* currentNode = this->m_Dictionary[index]->m_root->m_Prev;

				while (currentNode != this->m_Dictionary[index]->m_root)
				{
					this->m_Dictionary[index]->removeLast();

					unsigned __int32 dynamValueIndex = 0;

					for (int i = 0; i < this->m_dynamAllocValuesRegistered; i++)
					{
						if (this->m_dynamAllocValues[dynamValueIndex] == currentNode->m_Element->m_Value)
						{
							Memory::deallocateDelete<Value>(*this->m_ValueAllocator, m_dynamAllocValues[dynamValueIndex]);
							this->m_dynamAllocValuesRegistered--;
						}
						else
						{
							dynamValueIndex++;
						}
					}

#ifdef DEBUG
					Memory::deallocateDelete<Key>(*this->m_parentAllocator, *currentNode->m_Element->m_Key)
#endif // DEBUG

					Memory::deallocateDelete<HashNode<Key, Value>>(*this->m_NodeAllocator, *currentNode->m_Element);

					currentNode = currentNode->m_Prev;
				}
#ifdef DEBUG
				assert(currentNode == this->m_Dictionary[index]->m_root);
#endif // DEBUG

				this->m_usedTableSpace--;
			}

			void EmptyDictionary()
			{
				for (unsigned int i = 0; i < this->m_tableSize; i++)
				{
					this->EmptyIndex(i);
				}

				for (unsigned int i = 0; i < this->m_dynamAllocNodesRegistered; i++)
				{
					Memory::deallocateDelete<HashNode<Key, Value>>(*this->m_NodeAllocator, *this->m_dynamAllocNodes[i]);
				}
				this->m_dynamAllocNodesRegistered = 0;

				if (this->m_ValueAllocator)
				{
					for (unsigned int i = 0; i < this->m_dynamAllocValuesRegistered; i++)
					{
						Memory::deallocateDelete<Value>(*this->m_ValueAllocator, *this->m_dynamAllocValues[i]);
					}
					this->m_dynamAllocValuesRegistered = 0;
				}
			}

			template<typename Key, typename Value>
			friend std::ostream& operator<<(std::ostream& stream, const OpenHashTable<Key, Value>& hashTable)
			{
				for (unsigned int i = 0; i < hashTable.m_tableSize; i++)
				{
					stream << hashTable.m_Dictionary[i] << "\n";
				}

				return stream;
			}

			LinkedList<HashNode<Key, Value>>& operator[](const int& index)
			{
				return this->m_Dictionary[index];
			}

			private:
				void ClearDynamAllocs()
				{
					for (unsigned int i = 0; i < this->m_dynamAllocNodesRegistered; i++)
					{
#ifdef DEBUG
						Memory::deallocateDelete<Key>(*this->m_parentAllocator, *this->m_dynamAllocNodes[i].m_Key)
#endif // DEBUG

						Memory::deallocateDelete<HashNode<Key, Value>>(*this->m_NodeAllocator, *this->m_dynamAllocNodes[i]);
					}
					this->m_dynamAllocNodesRegistered = 0;

					if (this->m_ValueAllocator)
					{
						for (unsigned int i = 0; i < this->m_dynamAllocValuesRegistered; i++)
						{
							Memory::deallocateDelete<Value>(*this->m_ValueAllocator, *this->m_dynamAllocValues[i]);
						}
						this->m_dynamAllocValuesRegistered = 0;
					}
				}
		};
	}
}