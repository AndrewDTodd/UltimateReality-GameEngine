#pragma once

#include "../MemoryAllocation/MemoryManager.h"
#include "../MemoryAllocation/PoolAllocator.h"
#include "../MemoryAllocation/FreeListAllocator.h"
#include <iostream>

namespace Ultrality
{
	namespace StringTools
	{
		//class Memory::MemoryManager;

		class StringManager
		{
			friend class LocalString;
			friend class StringList;
			friend class StringTable;
			friend class UltraString;
		private:
			static StringManager* p_SingletonInstance;

			//list of managers that this system is dependent on

			//pointer to the memory manager
			Memory::MemoryManager* p_MemoryManager = nullptr;

			StringManager();
			~StringManager();

			//static allocator for all LocalString objects to allocate from
			Ultrality::Memory::FreeListAllocator* LocalStringAllocator = nullptr;

			//used for alocation handling in methods
			char* m_forMethodeAlloc = nullptr;

			//used for creating lists read from files that are of an undeturmined size
			StringList* m_ListForFileReading = nullptr;

			//used for allocating strings in a string list
			Memory::FreeListAllocator* m_stringAllocatorForStringLists = nullptr;

			//class dynamic allocator for StringTable
			Ultrality::Memory::FreeListAllocator* StringTableAllocator = nullptr;

			//static allocator for all UltraString objects to allocate from
			Ultrality::Memory::FreeListAllocator* UltraStringAllocator = nullptr;

			friend std::ostream& operator<<(std::ostream& stream, const UltraString& string);
			friend std::istream& operator>>(std::istream& stream, LocalString& string);
			friend std::istream& operator>>(std::istream& stream, StringList& list);

		public:
			static StringManager* getInstance();

			void startUp();
			void shutDown();

			//do not attempt to copy, manager is a singleton
			StringManager(const StringManager& manager) = delete;
			//do not attempt to copy, manager is a singleton
			void operator=(const StringManager& manager) = delete;
			//do not attempt to copy, manager is a singleton
			StringManager(StringManager&& manager) = delete;
			//do not attempt to copy, manager is a singleton
			void operator=(StringManager&& manager) = delete;
		};
	}
}