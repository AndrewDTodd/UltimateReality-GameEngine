#include "FileProcessor.h"
#include <assert.h>

namespace Ultrality
{
	namespace FileIO
	{
#ifdef DEBUG
		unsigned __int16 FileProcessor::m_requestsMade = 0;
#endif // DEBUG
		unsigned __int16 FileProcessor::maxNumberOfFileRequests = 0;

		unsigned __int8 FileProcessor::currentProcessor = 0;

		FileManager* FileProcessor::p_FileManager = nullptr;
		Memory::MemoryManager* FileProcessor::p_MemoryManager = nullptr;
		

		FileProcessor::FileProcessor()
		{
			this->m_ProcessRequests = true;
			this->m_file = std::fstream();

			void* p = this->p_MemoryManager->g_DynamicMemoryManager->allocate((sizeof(FileIO::FileRequest) * (maxNumberOfFileRequests / this->p_FileManager->maxNumberOfFileThreads)) + sizeof(Memory::LinkedList<FileRequest>), __alignof(Memory::LinkedList<FileRequest>));
			this->m_FileProcessorRequests = new (p) Memory::LinkedList<FileRequest>((sizeof(FileIO::FileRequest) * (maxNumberOfFileRequests / this->p_FileManager->maxNumberOfFileThreads)), Memory::pointer_math::add(p, sizeof(Memory::LinkedList<FileRequest>)));
		}
		FileProcessor::FileProcessor(FileProcessor&& processor)
		{
			if (processor.m_FileProcessorRequests->m_listLength == 0)
			{
				this->m_ProcessRequests = processor.m_ProcessRequests;
				this->m_file = std::fstream();
				processor.m_ProcessRequests = false;
				this->m_FileProcessorRequests = processor.m_FileProcessorRequests;
				processor.m_FileProcessorRequests = nullptr;
				processor.~FileProcessor();
			}
		}

		FileProcessor::~FileProcessor()
		{
#ifdef DEBUG
			assert(!this->m_ProcessRequests);
#endif // DEBUG
			if(this->m_FileProcessorRequests)
				Memory::deallocateDelete<Memory::LinkedList<FileRequest>>(*this->p_MemoryManager->g_DynamicMemoryManager, *this->m_FileProcessorRequests);
		}

		void FileProcessor::ProcessRequests()
		{
			while (this->m_ProcessRequests || this->m_FileProcessorRequests->m_listLength != 0)
			{
				if (this->m_FileProcessorRequests->m_root.m_Next)
				{
					if (this->m_FileProcessorRequests->m_root.m_Next->m_Element->m_file->m_format == 0)
					{
						switch (this->m_FileProcessorRequests->m_root.m_Next->m_Element->m_requestType)
						{
						case 0:
							this->m_file.open(this->m_FileProcessorRequests->m_root.m_Next->m_Element->m_FileLocation.m_charArray, std::fstream::out);

							this->m_file << *this->m_FileProcessorRequests->m_root.m_Next->m_Element->m_file;

							//Filethis::p_FileManager->m_FileProcessorRequests->m_root.m_Next->m_Element->m_file->~FileFormat();
							this->m_file.close();
							this->m_FileProcessorRequests->m_root.m_Next->m_Element->~FileRequest();
							this->m_FileProcessorRequests->removeFirst();
							break;
						case 1:
							this->m_file.open(this->m_FileProcessorRequests->m_root.m_Next->m_Element->m_FileLocation.m_charArray);

							this->m_file >> *this->m_FileProcessorRequests->m_root.m_Next->m_Element->m_file;

							//Filethis::p_FileManager->m_FileProcessorRequests->m_root.m_Next->m_Element->m_file->~FileFormat();
							this->m_file.close();
							this->m_FileProcessorRequests->m_root.m_Next->m_Element->~FileRequest();
							this->m_FileProcessorRequests->removeFirst();
							break;
						}
					}
					else
					{
						switch (this->m_FileProcessorRequests->m_root.m_Next->m_Element->m_requestType)
						{
						case 0:
							this->m_file.open(this->m_FileProcessorRequests->m_root.m_Next->m_Element->m_FileLocation.m_charArray, std::fstream::out, std::fstream::binary);

							this->m_file << *this->m_FileProcessorRequests->m_root.m_Next->m_Element->m_file;

							//Filethis::p_FileManager->m_FileProcessorRequests->m_root.m_Next->m_Element->m_file->~FileFormat();
							this->m_file.close();
							this->m_FileProcessorRequests->m_root.m_Next->m_Element->~FileRequest();
							this->m_FileProcessorRequests->removeFirst();
							break;
						case 1:
							this->m_file.open(this->m_FileProcessorRequests->m_root.m_Next->m_Element->m_FileLocation.m_charArray,std::fstream::in, std::fstream::binary);

							this->m_file >> *this->m_FileProcessorRequests->m_root.m_Next->m_Element->m_file;

							//Filethis::p_FileManager->m_FileProcessorRequests->m_root.m_Next->m_Element->m_file->~FileFormat();
							this->m_file.close();
							this->m_FileProcessorRequests->m_root.m_Next->m_Element->~FileRequest();
							this->m_FileProcessorRequests->removeFirst();
							break;
						}
					}
				}
			}
		}

		void FileProcessor::IssueRequest(FileRequest* request)
		{
			if (FileProcessor::p_FileManager->m_FileProcessors[FileProcessor::currentProcessor].m_FileProcessorRequests->m_listLength == 0)
			{
				FileProcessor::p_FileManager->m_FileProcessors[currentProcessor].m_FileProcessorRequests->AddFirst(request);
#ifdef DEBUG
				FileProcessor::m_requestsMade++;
#endif // DEBUG
			}
			else
			{
				if (request->m_requestPriority == IMMEDIATE)
				{
					FileProcessor::p_FileManager->m_FileProcessors[currentProcessor].m_FileProcessorRequests->InsertNode(request, 1);
#ifdef DEBUG
					FileProcessor::m_requestsMade++;
#endif // DEBUG
				}
				else
				{
					size_t listLength = FileProcessor::p_FileManager->m_FileProcessors[currentProcessor].m_FileProcessorRequests->m_listLength;
					listLength /= 5;
					switch (request->m_requestPriority)
					{
					case 1:
						FileProcessor::p_FileManager->m_FileProcessors[currentProcessor].m_FileProcessorRequests->InsertNode(request, listLength + 1);
#ifdef DEBUG
						FileProcessor::m_requestsMade++;
#endif // DEBUG

						break;
					case 2:
						FileProcessor::p_FileManager->m_FileProcessors[currentProcessor].m_FileProcessorRequests->InsertNode(request, (listLength * 2) + 1);
#ifdef DEBUG
						FileProcessor::m_requestsMade++;
#endif // DEBUG
						break;
					case 3:
						FileProcessor::p_FileManager->m_FileProcessors[currentProcessor].m_FileProcessorRequests->InsertNode(request, (listLength * 3) + 1);
#ifdef DEBUG
						FileProcessor::m_requestsMade++;
#endif // DEBUG
						break;
					case 4:
						FileProcessor::p_FileManager->m_FileProcessors[currentProcessor].m_FileProcessorRequests->InsertNode(request, (listLength * 4) + 1);
#ifdef DEBUG
						FileProcessor::m_requestsMade++;
#endif // DEBUG
						break;
					case 5:
						FileProcessor::p_FileManager->m_FileProcessors[currentProcessor].m_FileProcessorRequests->PrependNode(request);
#ifdef DEBUG
						FileProcessor::m_requestsMade++;
#endif // DEBUG
						break;
					}
				}
			}

			if (FileProcessor::currentProcessor != FileProcessor::p_FileManager->maxNumberOfFileThreads)
				FileProcessor::currentProcessor++;
			else
				FileProcessor::currentProcessor = 0;
		}

		void FileProcessor::operator()()
		{
			
		}
	}
}