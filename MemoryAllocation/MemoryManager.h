#pragma once

#include "StackAllocator.h"
#include "FreeListAllocator.h"

#define TOTAL_GIBIBYTES_FOR_SYSTEM 4
#define TOTAL_GIBIBYTES_FOR_DYNAMICMEM 2

namespace Ultrality
{
	namespace Memory
	{
		static const unsigned __int64 GibiByte = 1073741824;
		static const unsigned __int64 MebiByte = 1048576;
		static const unsigned __int64 KibiByte = 1024;

		class MemoryManager
		{
		private:
			void* g_ApplicationMemory = nullptr;
			//used to track singleton
			static MemoryManager* p_SingletonInstance;

			MemoryManager();
			~MemoryManager();
		public:
			StackAllocator* g_ApplicationMemoryManager = nullptr;

			FreeListAllocator* g_DynamicMemoryManager = nullptr;

			//static function that will construct a singleton instance of the Manager
			static MemoryManager* getInstance();

			//startUp will initialize caller
			void startUp();
			//shutDown will shut down and destroy object
			void shutDown();

			//do not attempt to copy, manager is a singleton
			MemoryManager(const MemoryManager& manager) = delete;
			//do not attempt to copy, manager is a singleton
			void operator=(const MemoryManager& manager) = delete;
			//do not attempt to copy, manager is a singleton
			MemoryManager(MemoryManager&& manager) = delete;
			//do not attempt to copy, manager is a singleton
			void operator=(MemoryManager&& manager) = delete;
		};
	}
}