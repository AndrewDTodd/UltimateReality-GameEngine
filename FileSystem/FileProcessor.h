#pragma once

#include "FileRequest.h"
#include <fstream>
#include "FileManager.h"
#include "FileFormats\FileFormat.h"
#include "../StringTools/StringManager.h"
#include "../MemoryAllocation/Allocator.h"

namespace Ultrality
{
	namespace FileIO
	{
		class FileProcessor
		{
			friend class FileRequest;
			friend class FileManager;
			friend class Memory::Allocator;

		public:
			//constructor activates list of requests, and thread that runs the ProcessRequests method
			FileProcessor();

			//move constructor
			FileProcessor(FileProcessor&& processor);

			//destructor that will detach the worker thread and destroy the queuse
			~FileProcessor();

			//fstream object used for input and output operations
			std::fstream m_file;

			static FileManager* p_FileManager;
			static Memory::MemoryManager* p_MemoryManager;

			static unsigned __int16 maxNumberOfFileRequests;

#ifdef DEBUG
			//holds the number of files that were registered
			//used for finalizing memory allotments
			static unsigned __int16 m_requestsMade;
#endif // DEBUG

			//function that workes through the request queue and processes the requests
			void ProcessRequests();

			bool m_ProcessRequests;

			static void IssueRequest(FileRequest* request);

			static unsigned __int8 currentProcessor;

			//queues that will hold a colective total of 1000 file requests by default
			//queues will be owned and worked by their respective FileProcessor objects
			//configured by the "iMaxNumberOfFileThreads" variable in "../UltralityEngine/UltralityCore/Config/EngineStartConfig.INI"
			Memory::LinkedList<FileRequest>* m_FileProcessorRequests = nullptr;

			
			//do not attempt to copy
			FileProcessor(FileProcessor& processor) = delete;
			//do not attempt to copy
			void operator=(FileProcessor& processor) = delete;

			//void operator=(FileProcessor&& processor);
			//FileProcessor* operator()();
			void operator()();
		};
	}
}