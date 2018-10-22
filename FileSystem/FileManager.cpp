#include "FileManager.h"
#include "../MemoryAllocation/MemoryManager.h"
#include "FileFormats\ConfigText\INI.h"
#include "../MemoryAllocation/StackAllocator.h"
#include "FileProcessor.h"
#include "../GlobalUtilities/GlobalUtilities.h"

namespace Ultrality
{
	namespace FileIO
	{
		FileManager* FileManager::p_SingletonInstance = nullptr;

		FileManager::FileManager()
		{

		}
		FileManager::~FileManager()
		{

		}

		FileManager* FileManager::getInstance()
		{
			if (!FileManager::p_SingletonInstance)
			{
				static FileManager s_singleton;
				return &s_singleton;
			}
			else
			{
				return FileManager::p_SingletonInstance;
			}
		}

		void FileManager::CreateINI()
		{
			static FileIO::INI config(StringTools::LocalString("EngineStartConfig"), StringTools::LocalString("../UltralityCore/Config"), 1);

			config.DefineSegment(StringTools::LocalString("FileSystem"), 2);

			config.DefineVariable(0, StringTools::LocalString("iMaxNumberOfFileThreads"), &maxNumberOfFileThreads);

			config.DefineVariable(0, StringTools::LocalString("iMaxNumberOfFileRequests"), &FileProcessor::maxNumberOfFileRequests);

			this->EngineStartConfig = &config;
		}

		void FileManager::startUp()
		{
			this->p_MemoryManager = Memory::MemoryManager::getInstance();

			void* p = this->p_MemoryManager->g_DynamicMemoryManager->allocate((sizeof(FileIO::Segment) * 50) + sizeof(Memory::FreeListAllocator), __alignof(Memory::FreeListAllocator));
			this->m_INIsegmentAllocator = new (p) Memory::FreeListAllocator((sizeof(Segment) * 50), Memory::pointer_math::add(p, sizeof(Memory::FreeListAllocator)));

			p = this->p_MemoryManager->g_DynamicMemoryManager->allocate((sizeof(StringTools::LocalString) * 200) + sizeof(Memory::FreeListAllocator), __alignof(Memory::FreeListAllocator));
			this->m_INIsegmentNameAllocator = new (p) Memory::FreeListAllocator((sizeof(StringTools::LocalString) * 200), Memory::pointer_math::add(p, sizeof(Memory::FreeListAllocator)));

			p = this->p_MemoryManager->g_DynamicMemoryManager->allocate((sizeof(void*) * 200) + sizeof(Memory::FreeListAllocator), __alignof(Memory::FreeListAllocator));
			this->m_INIsegmentPointerAllocator = new (p) Memory::FreeListAllocator((sizeof(void*) * 200), Memory::pointer_math::add(p, sizeof(Memory::FreeListAllocator)));

			p = this->p_MemoryManager->g_DynamicMemoryManager->allocate((MULTIPLY(Memory::MebiByte, 300) + sizeof(Memory::FreeListAllocator)), __alignof(Memory::FreeListAllocator));
			this->m_FilesObjectsAllocator = new (p) Memory::FreeListAllocator((MULTIPLY(Memory::MebiByte, 300)), Memory::pointer_math::add(p, sizeof(Memory::FreeListAllocator)));

			FileManager::p_SingletonInstance = this;

			FileProcessor::p_FileManager = this;
			FileProcessor::p_MemoryManager = this->p_MemoryManager;

			this->CreateINI();

			std::fstream file;
#ifdef RELEASE
			file.open("../../UltralityEngine_Core/UltralityCore/Config/EngineStartConfig.INI");
#endif

#ifdef DEBUG
			file.open("..//UltralityEngine_Core//UltralityCore//Config//EngineStartConfig.INI");
#endif

			file >> *this->EngineStartConfig;

			this->EngineStartConfig->~INI();

			file.close();

			bool test = file.is_open();

			this->m_FileProcessors = Memory::allocateArray<FileProcessor>(this->p_MemoryManager->g_DynamicMemoryManager, this->maxNumberOfFileThreads);

			this->m_asyncronousFileThreads = Memory::allocateArray<std::thread>(this->p_MemoryManager->g_DynamicMemoryManager, this->maxNumberOfFileThreads);

			for (unsigned __int8 i = 0; i < this->maxNumberOfFileThreads; i++)
			{
				this->m_asyncronousFileThreads[i] = std::thread(&FileProcessor::ProcessRequests, &this->m_FileProcessors[i]);
			}
		}

		void FileManager::shutDown()
		{
			for (unsigned __int8 i = 0; i < this->maxNumberOfFileThreads; i++)
			{
				this->m_FileProcessors[i].m_ProcessRequests = false;
				while (this->m_FileProcessors[i].m_FileProcessorRequests->m_listLength != 0)
				{

				}

				this->m_asyncronousFileThreads[i].detach();
				this->m_asyncronousFileThreads[i].~thread();
			}
			

			Memory::deallocateArray<std::thread>(this->p_MemoryManager->g_DynamicMemoryManager, this->m_asyncronousFileThreads);

			Memory::deallocateArray<FileProcessor>(this->p_MemoryManager->g_DynamicMemoryManager, this->m_FileProcessors);

			Memory::deallocateDelete<Memory::FreeListAllocator>(*this->p_MemoryManager->g_DynamicMemoryManager, *this->m_FilesObjectsAllocator);
			Memory::deallocateDelete<Memory::FreeListAllocator>(*this->p_MemoryManager->g_DynamicMemoryManager, *this->m_INIsegmentPointerAllocator);
			Memory::deallocateDelete<Memory::FreeListAllocator>(*this->p_MemoryManager->g_DynamicMemoryManager, *this->m_INIsegmentNameAllocator);
			Memory::deallocateDelete<Memory::FreeListAllocator>(*this->p_MemoryManager->g_DynamicMemoryManager, *this->m_INIsegmentAllocator);

			this->p_MemoryManager = nullptr;

			FileManager::p_SingletonInstance = nullptr;

			this->~FileManager();
		}
	}
}