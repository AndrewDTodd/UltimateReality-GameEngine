#pragma once

#include "../Memory.h"
#include <iostream>
#include <assert.h>
#include "../../Math/RNG.h"
#include <iomanip>

namespace Ultrality
{
	namespace Memory
	{
		template<typename Key, typename Value>
		class TreeNode
		{
			template<typename Key, typename Value>
			friend class BinaryTree;
		private:
			TreeNode<Key, Value>* m_left;
			TreeNode<Key, Value>* m_right;
			TreeNode<Key, Value>* m_parent;

			Key m_Key;
			Value* m_Value;
			TreeNode()
			{
				this->m_left = nullptr;
				this->m_right = nullptr;
				this->m_Value = nullptr;
				this->m_parent = nullptr;
				this->m_Key = 0;
			}
			~TreeNode()
			{
				this->m_left = nullptr;
				this->m_right = nullptr;
				this->m_Value = nullptr;
				this->m_parent = nullptr;
			}
		public:
			template<typename Key, typename Value>
			friend std::ostream& operator<<(std::ostream& stream, const TreeNode<Key, Value>& node)
			{
				if (node.m_Value)
					stream << "|" << std::setw(18) << node.m_Key << std::setw(18) << "|" << "\n" << *node.m_Value;
				else
					stream << "|" << std::setw(18) << node.m_Key << std::setw(18) << "|" << "\n";
				return stream;
			}
		};

		template<typename Key, typename Value>
		class BinaryTree
		{
		private:
			Allocator* m_parentAllocator;
		public:
			PoolAllocator* m_NodeAllocator;
			PoolAllocator* m_ValueAllocator;

			TreeNode<Key, Value>* m_root;

			BinaryTree(const size_t& nodeAllocatorSize, Allocator* allocator)
			{
				void* p = allocator->allocate(nodeAllocatorSize + sizeof(PoolAllocator), __alignof(PoolAllocator));
				this->m_NodeAllocator = new (p) PoolAllocator(sizeof(TreeNode<Key, Value>), alignof(TreeNode<Key, Value>), nodeAllocatorSize, pointer_math::add(p, sizeof(PoolAllocator)));
				this->m_ValueAllocator = nullptr;

				this->m_root = Memory::allocateNew<TreeNode<Key, Value>>(sizeof(TreeNode<Key, Value>), *this->m_NodeAllocator, TreeNode<Key, Value>());

				this->m_parentAllocator = allocator;
			#ifdef DEBUG
				assert(this->m_root && this->m_root->m_Key == 0);
			#endif // DEBUG

			}
			BinaryTree(const size_t& nodeAllocatorSize, const size_t& valueAllocatorSize, Allocator* allocator)
			{
				void* p = allocator->allocate(nodeAllocatorSize + sizeof(PoolAllocator), __alignof(PoolAllocator));
				this->m_NodeAllocator = new (p) PoolAllocator(sizeof(TreeNode<Key, Value>), alignof(TreeNode<Key, Value>), nodeAllocatorSize, pointer_math::add(p, sizeof(PoolAllocator)));

				this->m_root = Memory::allocateNew<TreeNode<Key, Value>>(sizeof(TreeNode<Key, Value>), *this->m_NodeAllocator, TreeNode<Key, Value>());
			#ifdef DEBUG
				assert(this->m_root && this->m_root->m_Key == 0);
			#endif // DEBUG

				if (valueAllocatorSize != 0)
				{
					void* p = allocator->allocate(valueAllocatorSize + sizeof(PoolAllocator), __alignof(PoolAllocator));
					this->m_ValueAllocator = new (p) PoolAllocator(sizeof(Value), alignof(Value), valueAllocatorSize, pointer_math::add(p, sizeof(PoolAllocator)));
				}
				else
				{
					this->m_ValueAllocator = nullptr;
				}

				this->m_parentAllocator = allocator;
			}
			BinaryTree(const size_t& nodeAllocatorSize, Allocator* allocator, Key* keys, Value* elements, const size_t& arraySize)
			{
				void* p = allocator->allocate(nodeAllocatorSize + sizeof(PoolAllocator), __alignof(PoolAllocator));
				this->m_NodeAllocator = new (p) PoolAllocator(sizeof(TreeNode<Key, Value>), alignof(TreeNode<Key, Value>), nodeAllocatorSize, pointer_math::add(p, sizeof(PoolAllocator)));
				this->m_ValueAllocator = nullptr;

				this->m_root = Memory::allocateNew<TreeNode<Key, Value>>(sizeof(TreeNode<Key, Value>), *this->m_NodeAllocator, TreeNode<Key, Value>());
			#ifdef DEBUG
				assert(this->m_root && this->m_root->m_Key == 0);
			#endif // DEBUG

				this->m_root->m_Key = keys[0];
				this->m_root->m_Value = elements[0];

				for (int i = 1; i < arraySize; i++)
				{
					this->Insert(keys[i], elements[i]);
				}

				this->m_parentAllocator = allocator;
			}

			~BinaryTree()
			{
				if (!this->m_ValueAllocator)
				{
					Memory::deallocateDelete<PoolAllocator>(*this->m_parentAllocator, *this->m_NodeAllocator);
					this->m_NodeAllocator = nullptr;
					Memory::deallocateDelete<PoolAllocator>(*this->m_parentAllocator, *this->m_ValueAllocator);
					this->m_ValueAllocator = nullptr;
				}
				else
				{
#ifdef DEBUG
					assert(!this->m_ValueAllocator);
#endif // DEBUG

					Memory::deallocateDelete<PoolAllocator>(*this->m_parentAllocator, *this->m_NodeAllocator);
					this->m_NodeAllocator = nullptr;
				}
			}

			void Insert(const Key& key, Value* value)
			{
				#ifdef DEBUG
					assert(this->m_ValueAllocator == nullptr);
				#endif // DEBUG
				static TreeNode<Key, Value>* newNode;
				newNode = Memory::allocateNew<TreeNode<Key, Value>>(sizeof(TreeNode<Key, Value>), *this->m_NodeAllocator, TreeNode<Key, Value>());
				newNode->m_Key = key;
				newNode->m_Value = value;
				TreeNode<Key, Value>* testNode = this->m_root;

				while (testNode)
				{
					if (newNode->m_Key < testNode->m_Key)
					{
						newNode->m_parent = testNode;
						testNode = testNode->m_left;
					}
					else
					{
						newNode->m_parent = testNode;
						testNode = testNode->m_right;
					}
				}

				testNode = newNode;
				newNode = nullptr;
			}
			void InsertDynam(const Key& key, Value& value)
			{
				#ifdef DEBUG
					assert(this->m_ValueAllocator);
				#endif // DEBUG
				static TreeNode<Key, Value>* newNode;
				newNode = Memory::allocateNew<TreeNode<Key, Value>>(sizeof(TreeNode<Key, Value>), *this->m_NodeAllocator, TreeNode<Key, Value>());
				newNode->m_Key = key;
				newNode->m_Value = Memory::allocateNew<Value>(sizeof(Value), *this->m_ValueAllocator, value);
				TreeNode<Key, Value>* testNode = this->m_root;

				while (testNode)
				{
					if (newNode->m_Key < testNode->m_Key)
					{
						newNode->m_parent = testNode;
						testNode = testNode->m_left;
						if (testNode == nullptr)
						{
							testNode = newNode;
							newNode->m_parent->m_left = newNode;
							testNode = nullptr;
							newNode = nullptr;
						}
					}
					else
					{
						newNode->m_parent = testNode;
						testNode = testNode->m_right;
						if (testNode == nullptr)
						{
							testNode = newNode;
							newNode->m_parent->m_right = newNode;
							testNode = nullptr;
							newNode = nullptr;
						}
					}
				}

				testNode = newNode;
				newNode = nullptr;
			}

			TreeNode<Key, Value>* Search(const Key& key)
			{
				TreeNode<Key, Value>* testNode = this->m_root;

				while (testNode->m_Key != key)
				{
					if (key < testNode->m_Key)
					{
						testNode = testNode->m_left;
					}
					else
					{
						testNode = testNode->m_right;
					}
					#ifdef DEBUG
					if (!testNode)
					{
						std::cout << "No matching key" << std::endl;
						return nullptr;
					}
					#endif
				}

				return testNode;
			}
			TreeNode<Key, Value>* SearchSplay(const Key& key)
			{
				TreeNode<Key, Value>* testNode = this->m_root;

				while (testNode->m_Key != key)
				{
					if (key < testNode->m_Key)
					{
						testNode = testNode->m_left;
					}
					else
					{
						testNode = testNode->m_right;
					}
					#ifdef DEBUG
					if (!testNode)
					{
						std::cout << "No matching key" << std::endl;
						return nullptr;
					}
					#endif
					}

				this->Splay(testNode);

				#ifdef DEBUG
					assert(testNode == this->m_root);
				#endif // DEBUG

				return this->m_root;
			}

			void RemoveNode(const Key& key)
			{
			#ifdef DEBUG
				assert(!this->m_valueAllocator);
			#endif // DEBUG

				TreeNode<Key, Value>* testNode = this->m_root;

				while (testNode->m_Key != key)
				{
					if (key < testNode->m_Key)
					{
						testNode = testNode->m_left;
					}
					else
					{
						testNode = testNode->m_parent;
					}
					#ifdef DEBUG
					if (!testNode)
					{
						std::cout << "No matching key" << std::endl;
						return;
					}
					#endif
				}

				#ifdef DEBUG
					assert(testNode);
				#endif // DEBUG
				if (testNode->m_left && testNode->m_right)
				{
					TreeNode<Key, Value>* childNode;
					switch (double v = Math::RNG::MOA32())
					{
					case v < 0.5:
						childNode = testNode->m_left;
					case v >= 0.5:
						childNode = testNode->m_right;
					}

					testNode = childNode;
					RemoveNode(childNode->m_Key);
				}

				if (!testNode->m_left || !testNode->m_right)
				{
					if (testNode == testNode->m_parent->m_left)
					{
						if (testNode->m_right)
						{
							testNode->m_parent->m_left = testNode->m_right;
							testNode->~TreeNode();
						}
						else
						{
							testNode->m_parent->m_left = testNode->m_left;
							testNode->~TreeNode();
						}
					}
					else
					{
						if (testNode->m_right)
						{
							testNode->m_parent->m_right = testNode->m_right;
							testNode->~TreeNode();
						}
						else
						{
							testNode->m_parent->m_right = testNode->m_left;
							testNode->~TreeNode();
						}
					}
				}

				if (!testNode->m_left && !testNode->m_right)
				{
					if (testNode == testNode->m_parent->m_left)
					{
						testNode->m_parent->m_left = nullptr;
						testNode->~TreeNode();
					}
					else
					{
						testNode->m_parent->m_right = nullptr;
						testNode->~TreeNode();
					}
				}
			}
			void RemoveNodeDeleteDynam(const Key& key)
			{
				#ifdef DEBUG
					assert(this->m_valueAllocator);
				#endif // DEBUG

				TreeNode<Key, Value>* testNode = this->m_root;

				while (testNode->m_Key != key)
				{
					if (key < testNode->m_Key)
					{
						testNode = testNode->m_parent->m_left;
					}
					else
					{
						testNode = testNode->m_parent->m_right;
					}
				#ifdef DEBUG
					if (!testNode)
					{
						std::cout << "No matching key" << std::endl;
						return;
					}
				#endif
				}

				#ifdef DEBUG
					assert(testNode);
				#endif // DEBUG
				if (testNode->m_left && testNode->m_right)
				{
					TreeNode<Key, Value>* childNode;
					switch (double v = Math::RNG::MOA32())
					{
					case v < 0.5:
						childNode = testNode->m_left;
					case v >= 0.5:
						childNode = testNode->m_right;
					}

					testNode = childNode;
					RemoveNode(childNode->m_Key);
				}

				if (!testNode->m_left || !testNode->m_right)
				{
					if (testNode == testNode->m_parent->m_left)
					{
						if (testNode->m_right)
						{
							testNode->m_parent->m_left = testNode->m_right;
							Memory::deallocateDelete<Value>(*this->m_ValueAllocator, *testNode->m_Value);
							testNode->~TreeNode();
						}
						else
						{
							testNode->m_parent->m_left = testNode->m_left;
							Memory::deallocateDelete<Value>(*this->m_ValueAllocator, *testNode->m_Value);
							testNode->~TreeNode();
						}
					}
					else
					{
						if (testNode->m_right)
						{
							testNode->m_parent->m_right = testNode->m_right;
							Memory::deallocateDelete<Value>(*this->m_ValueAllocator, *testNode->m_Value);
							testNode->~TreeNode();
						}
						else
						{
							testNode->m_parent->m_right = testNode->m_left;
							Memory::deallocateDelete<Value>(*this->m_ValueAllocator, *testNode->m_Value);
							testNode->~TreeNode();
						}
					}
				}

				if (!testNode->m_left && !testNode->m_right)
				{
					if (testNode == testNode->m_parent->m_left)
					{
						testNode->m_parent->m_left = nullptr;
						Memory::deallocateDelete<Value>(*this->m_ValueAllocator, *testNode->m_Value);
						testNode->~TreeNode();
					}
					else
					{
						testNode->m_parent->m_right = nullptr;
						Memory::deallocateDelete<Value>(*this->m_ValueAllocator, *testNode->m_Value);
						testNode->~TreeNode();
					}
				}
			}

			void TraverseTree(TreeNode<Key, Value>* node, void(*function)(TreeNode<Key, Value>*))
			{
				if (!node)
				{
					return;
				}

				TraverseTree<Type...>(node->m_left, function);
				function(node);
				TraverseTree<Type...>(node->m_right, function);
			}

			BinaryTree<Key, Value>* Split(TreeNode<Key, Value>* node, const size_t& nodeAllocatorSize, const size_t& valueAllocatorSize, Allocator* allocator)
			{
				BinaryTree<Key, Value>* newTree = Memory::allocateNew<BinaryTree<Key, Value>>(sizeof(BinaryTree<Key, Value>), allocator, BinaryTree<Key, Value>(nodeAllocatorSize, valueAllocatorSize, allocator));

				this->Splay(node);
#ifdef DEBUG
				assert(node == this->m_root);
#endif // DEBUG
				this->m_root = node->m_right;
				this->m_root->m_left = nullptr;
				node->m_right = nullptr;
				newTree->m_root = node;
				return newTree;
			}

			BinaryTree<Key, Value>* Join(BinaryTree<Key, Value>* smallValues, BinaryTree<Key, Value>* largeValues, const size_t& nodeAllocatorSize, const size_t& valueAllocatorSize, Allocator* allocator)
			{
				BinaryTree<Key, Value>* newTree = Memory::allocateNew<BinaryTree<Key, Value>>(sizeof(BinaryTree<Key, Value>), allocator, BinaryTree<Key, Value>(nodeAllocatorSize, valueAllocatorSize, allocator));
#ifdef DEBUG
				assert(smallValues->m_root->m_right == nullptr);
#endif // DEBUG
				smallValues->Splay(smallValues->MaxNode());
#ifdef DEBUG
				assert(smallValues->m_root->m_right == nullptr);
#endif // DEBUG
				smallValues->m_root->m_right = largeValues->m_root;
				newTree->m_root = smallValues->m_root;
				return newTree;
			}

			TreeNode<Key, Value>* MinNode()
			{
				TreeNode<Key, Value>* node = this->m_root;
				while (node->m_left)
				{
					node = node->m_left;
				}
				return node;
			}
			TreeNode<Key, Value>* MaxNode()
			{
				TreeNode<Key, Value>* node = this->m_root;
				while (node->m_right)
				{
					node = node->m_right;
				}
				return node;
			}

			template<typename Key, typename Value>
			friend std::ostream& operator<<(std::ostream& stream, BinaryTree<Key, Value>& tree)
			{
				TreeNode<Key, Value>* node = tree.m_root;
				TreeNode<Key, Value>* rowEnd = tree.m_root;

				tree.TraverseTreeForOut(&stream, node, rowEnd);
				return stream;
			}

			__int16 findTreeWidth()
			{
				TreeNode<Key, Value>* node = this->m_root;
				__int16 count = 1;
				while (node->m_left)
				{
					count++;
					node = node->m_left;
				}
				node = this->m_root;
				while (node->m_right)
				{
					count++;
					node = node->m_right;
				}
				return count;
			}
			
			private:
			inline void ZigStep(TreeNode<Key, Value>* node)
			{
			#ifdef DEBUG
				assert(node && node->m_parent && node->m_parent == this->m_root);
			#endif // DEBUG
				if (node == node->m_parent->m_left)
				{
					node->m_parent->m_left = node->m_right;
					node->m_right = node->m_parent;
				}
				if (node == node->m_parent->m_right)
				{
					node->m_parent->m_right = node->m_left;
					node->m_left = node->m_parent;
				}
			}

			inline void ZigZigStep(TreeNode<Key, Value>* node)
			{
			#ifdef DEBUG
				assert(node->m_parent != this->m_root && ((node == node->m_parent->m_left && node->m_parent == node->m_parent->m_parent->m_left) || (node == node->m_parent->m_right && node->m_parent == node->m_parent->m_parent->m_right)));
			#endif // DEBUG
				if (node == node->m_parent->m_left)
				{
					TreeNode<Key, Value>* current = node;
					TreeNode<Key, Value>* parent = node->m_parent;
					TreeNode<Key, Value>* grandparent = node->m_parent->m_parent;

					node->m_parent->m_parent->m_left = node->m_parent->m_right;
					node->m_parent->m_left = node->m_right;
					node->m_right = node->m_parent;
					node->m_parent->m_right = node->m_parent->m_parent;

					node->m_parent->m_parent->m_parent = parent;
					node->m_parent->m_parent = current;
					node->m_parent = grandparent->m_parent;

					switch (grandparent == grandparent->m_parent->m_left)
					{
					case true:
						grandparent->m_parent->m_left = node;
					case false:
						grandparent->m_parent->m_right = node;
					}
				}
				else
				{
					TreeNode<Key, Value>* current = node;
					TreeNode<Key, Value>* parent = node->m_parent;
					TreeNode<Key, Value>* grandparent = node->m_parent->m_parent;

					node->m_parent->m_parent->m_right = node->m_parent->m_left;
					node->m_parent->m_right = node->m_left;
					node->m_left = node->m_parent;
					node->m_parent->m_left = node->m_parent->m_parent;

					node->m_parent->m_parent->m_parent = parent;
					node->m_parent->m_parent = current;
					node->m_parent = grandparent->m_parent;

					switch (grandparent == grandparent->m_parent->m_left)
					{
					case true:
						grandparent->m_parent->m_left = node;
					case false:
						grandparent->m_parent->m_right = node;
					}
				}
			}

			inline void ZigZagStep(TreeNode<Key, Value>* node)
			{
			#ifdef DEBUG
				assert(node->m_parent != this->m_root && ((node == node->m_parent->m_left && node->m_parent == node->m_parent->m_parent->m_right) || (node == node->m_parent->m_right && node->m_parent == node->m_parent->m_parent->m_left)));
			#endif // DEBUG
				if (node == node->m_parent->m_left)
				{
					TreeNode<Key, Value>* current = node;
					TreeNode<Key, Value>* parent = node->m_parent;
					TreeNode<Key, Value>* grandparent = node->m_parent->m_parent;

					node->m_parent->m_left = node->m_right;
					node->m_parent->m_parent->m_right = node->m_left;
					node->m_right = node->m_parent;
					node->m_left = node->m_parent->m_parent;

					node->m_parent->m_parent->m_parent = current;
					node->m_parent->m_parent = curent;
					node->m_parent = grandparent->m_parent;

					switch (grandparent == grandparent->m_parent->m_left)
					{
					case true:
						grandparent->m_parent->m_left = node;
					case false:
						grandparent->m_parent->m_right = node;
					}
				}
				else
				{
					TreeNode<Key, Value>* current = node;
					TreeNode<Key, Value>* parent = node->m_parent;
					TreeNode<Key, Value>* grandparent = node->m_parent->m_parent;

					node->m_parent->m_right = node->m_left;
					node->m_parent->m_parent->m_left = node->m_right;
					node->m_left = node->m_parent;
					node->m_right = node->m_parent->m_parent;

					node->m_parent->m_parent->m_parent = current;
					node->m_parent->m_parent = curent;
					node->m_parent = grandparent->m_parent;

					switch (grandparent == grandparent->m_parent->m_left)
					{
					case true:
						grandparent->m_parent->m_left = node;
					case false:
						grandparent->m_parent->m_right = node;
					}
				}
			}

			void Splay(TreeNode<Key, Value>* node)
			{
			#ifdef DEBUG
				assert((node && node->m_parent && node->m_parent == this->m_root) || (((node == node->m_parent->m_left && node->m_parent == node->m_parent->m_parent->m_right) || (((node == node->m_parent->m_left && node->m_parent == node->m_parent->m_parent->m_left) || (node == node->m_parent->m_right && node->m_parent == node->m_parent->m_parent->m_right))))));
			#endif // DEBUG

				while (node != this->m_root)
				{
					if (node->m_parent != this->m_root && ((node == node->m_parent->m_left && node->m_parent == node->m_parent->m_parent->m_right) || (node == node->m_parent->m_right && node->m_parent == node->m_parent->m_parent->m_left)))
					{
						this->ZigZagStep(node);
					}
					else if (node->m_parent != this->m_root && ((node == node->m_parent->m_left && node->m_parent == node->m_parent->m_parent->m_left) || (node == node->m_parent->m_right && node->m_parent == node->m_parent->m_parent->m_right)))
					{
						this->ZigZigStep(node);
					}
					else if (node && node->m_parent && node->m_parent == this->m_root)
					{
						this->ZigStep(node);
						break;
					}
				}

				#ifdef DEBUG
					assert(node == this->m_root);
				#endif // DEBUG
			}

			inline void genStream(std::ostream* stream, TreeNode<Key, Value>* node, TreeNode<Key, Value>* rowEnd)
			{
			    if (node == nullptr)
				{
					*stream << " " << std::setw(35);
					return;
				}
				else if (node == rowEnd)
				{
					if (rowEnd->m_right)
						rowEnd = rowEnd->m_right;
					else
						rowEnd = rowEnd->m_left;
					*stream << *node << "\n";
					return;
				}
				else
				{
					*stream << *node << std::setw(1);
					return;
				}
			}

			inline __int16 pow(__int16& num, __int16& pow)
			{
				for (int i = 0; i <= pow; i++)
				{
					num *= num;
				}
				return num;
			}

			inline void TraverseTreeForOut(std::ostream* stream, TreeNode<Key, Value>* node, TreeNode<Key, Value>* rowEnd)
			{
				this->genStream(stream, node, rowEnd);
				if (node == nullptr)
				{
					return;
				}
				if(node->m_left)
					TraverseTreeForOut(stream, node->m_left, rowEnd);
				if(node->m_right)
				TraverseTreeForOut(stream, node->m_right, rowEnd);
			}
		};
	}
}

