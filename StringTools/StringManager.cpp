#include "StringManager.h"
#include "LocalString.h"
#include "StringList.h"

namespace Ultrality
{
	namespace StringTools
	{
		StringManager* StringManager::p_SingletonInstance = nullptr;

		StringManager::StringManager()
		{

		}
		StringManager::~StringManager()
		{

		}

		StringManager* StringManager::getInstance()
		{
			if (!StringManager::p_SingletonInstance)
			{
				static StringManager s_singleton;
				return &s_singleton;
			}
			else
			{
				return StringManager::p_SingletonInstance;
			}
		}

		void StringManager::startUp()
		{
			this->p_MemoryManager = Memory::MemoryManager::getInstance();

			this->UltraStringAllocator = Memory::allocateNew<Memory::FreeListAllocator>((Memory::MebiByte * 300), this->p_MemoryManager->g_ApplicationMemoryManager);

			this->StringTableAllocator = Memory::allocateNew<Memory::FreeListAllocator>((Memory::MebiByte * 300), this->p_MemoryManager->g_DynamicMemoryManager);

			this->LocalStringAllocator = Memory::allocateNew<Memory::FreeListAllocator>((Memory::MebiByte * 300), this->p_MemoryManager->g_ApplicationMemoryManager);

			this->m_stringAllocatorForStringLists = Memory::allocateNew<Memory::FreeListAllocator>((sizeof(StringTools::LocalString) * 1000), this->p_MemoryManager->g_DynamicMemoryManager);
			
			this->m_forMethodeAlloc = Memory::allocateArray<char>(this->LocalStringAllocator, 65535);

			//this->m_ListForFileReading = new StringList(255);

			this->m_ListForFileReading = Memory::allocateNew<StringList>(sizeof(StringList), *this->p_MemoryManager->g_ApplicationMemoryManager, StringList(255));

			StringManager::p_SingletonInstance = this;
		}
		
		void StringManager::shutDown()
		{
			//this->m_ListForFileReading->~StringList();
			//delete this->m_ListForFileReading;

			Memory::deallocateDelete<StringList>(*this->p_MemoryManager->g_ApplicationMemoryManager, *this->m_ListForFileReading);

			Memory::deallocateArray<char>(this->LocalStringAllocator, this->m_forMethodeAlloc);
			this->m_forMethodeAlloc = nullptr;

			Memory::deallocateDelete<Memory::FreeListAllocator>(*this->p_MemoryManager->g_DynamicMemoryManager, *this->m_stringAllocatorForStringLists);

			Memory::deallocateDelete<Memory::FreeListAllocator>(*this->p_MemoryManager->g_ApplicationMemoryManager, *this->LocalStringAllocator);

			Memory::deallocateDelete<Memory::FreeListAllocator>(*this->p_MemoryManager->g_DynamicMemoryManager, *this->StringTableAllocator);

			Memory::deallocateDelete<Memory::FreeListAllocator>(*this->p_MemoryManager->g_ApplicationMemoryManager, *this->UltraStringAllocator);

			this->p_MemoryManager = nullptr;

			StringManager::p_SingletonInstance = nullptr;

			this->~StringManager();
		}
	}
}