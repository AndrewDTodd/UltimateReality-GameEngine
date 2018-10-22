#pragma once

#include "../MemoryAllocation/StackAllocator.h"
#include "../MemoryAllocation/Containers/LinkedList.h"
#include "FileRequest.h"
#include "../MemoryAllocation/MemoryManager.h"
#include <thread>

namespace Ultrality
{
	namespace FileIO
	{
		class FileManager
		{
			friend class INI;
			friend class FileProcessor;
		public:
			static FileManager* p_SingletonInstance;

			//list of managers that this system is dependent on

			//pointer to the memory manager
			Memory::MemoryManager* p_MemoryManager = nullptr;

			FileManager();
			~FileManager();

			//pool allocators used by the segments to allocate arrays of variable names in INI files
			Memory::FreeListAllocator* m_INIsegmentNameAllocator = nullptr;
			//used to allocate the arrays of void* for the segments in INI files
			Memory::FreeListAllocator* m_INIsegmentPointerAllocator = nullptr;
			//allocator for section creation in INI files
			Memory::FreeListAllocator* m_INIsegmentAllocator = nullptr;
			//allocator for any object held by a file object
			Memory::FreeListAllocator* m_FilesObjectsAllocator = nullptr;

			unsigned __int8 maxNumberOfFileThreads = 0;

			void CreateINI();

			INI* EngineStartConfig = nullptr;

			FileProcessor* m_FileProcessors = nullptr;

			//array of threads that will be spawned dependent on config
			//each thread will run a FileProcessor
			//configured by the "iMaxNumberOfFileThreads" variable in "../UltralityEngine/UltralityCore/Config/EngineStartConfig.INI"
			std::thread* m_asyncronousFileThreads = nullptr;

		public:
			//use to construct singleton instance
			static FileManager* getInstance();

			//will initialize the File System
			void startUp();

			//will shut down the File System
			void shutDown();

			//do not attempt to copy, manager is a singleton
			FileManager(const FileManager& manager) = delete;
			//do not attempt to copy, manager is a singleton
			void operator=(const FileManager& manager) = delete;
			//do not attempt to copy, manager is a singleton
			FileManager(FileManager&& manager) = delete;
			//do not attempt to copy, manager is a singleton
			void operator=(FileManager&& manager) = delete;
		};
	}
}