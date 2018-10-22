#include "MemoryManager.h"
#include <stdlib.h>
#include <new>

namespace Ultrality
{
	namespace Memory
	{
		MemoryManager* MemoryManager::p_SingletonInstance = nullptr;

		MemoryManager::MemoryManager()
		{

		}
		MemoryManager::~MemoryManager()
		{

		}

		MemoryManager* MemoryManager::getInstance()
		{
			if (!MemoryManager::p_SingletonInstance)
			{
				static MemoryManager s_singleton;
				return &s_singleton;
			}
			else
			{
				return MemoryManager::p_SingletonInstance;
			}
		}

		void MemoryManager::startUp()
		{
			this->g_ApplicationMemory = malloc(GibiByte * TOTAL_GIBIBYTES_FOR_SYSTEM);

			this->g_ApplicationMemoryManager = new (this->g_ApplicationMemory) StackAllocator(GibiByte * TOTAL_GIBIBYTES_FOR_SYSTEM, pointer_math::add(this->g_ApplicationMemory, sizeof(StackAllocator)));

			this->g_DynamicMemoryManager = allocateNew<FreeListAllocator>(GibiByte * TOTAL_GIBIBYTES_FOR_DYNAMICMEM, g_ApplicationMemoryManager);

			MemoryManager::p_SingletonInstance = this;
		}

		void MemoryManager::shutDown()
		{
			MemoryManager::p_SingletonInstance = nullptr;

			Memory::deallocateDelete<FreeListAllocator>(*this->g_ApplicationMemoryManager, *this->g_DynamicMemoryManager);
			this->g_ApplicationMemoryManager->~StackAllocator();
			this->g_ApplicationMemoryManager = nullptr;
			this->g_DynamicMemoryManager = nullptr;
			free(this->g_ApplicationMemory);
			this->g_ApplicationMemory = nullptr;

			this->~MemoryManager();
		}
	}
}