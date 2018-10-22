#include "FileRequest.h"
#include "FileProcessor.h"

namespace Ultrality
{
	namespace FileIO
	{
		FileRequest::FileRequest(RequestType&& type, RequestPriority&& priority, FileFormat* file)
		{
			this->m_file = file;
			this->m_requestType = type;
			this->m_requestPriority = priority;
			this->m_FileLocation = this->m_file->m_fileLocation;
			this->m_FileLocation += this->m_file->m_fileName;
			this->m_FileLocation += this->m_file->m_fileExtension;
			this->m_FileLocation.endLine();

			FileProcessor::IssueRequest(this);
		}
		FileRequest::~FileRequest()
		{
			if (this->m_file)
			{
				this->done = true;
				this->m_FileLocation.~LocalString();
				this->m_file->~FileFormat();
				this->m_file = nullptr;
			}
		}

		void FileRequest::WaitForFile()
		{
			while (!this->done)
			{

			}
		}

		std::ostream& operator<<(std::ostream& stream, const FileRequest& request)
		{
			stream << *request.m_file;

			return stream;
		}
		std::istream& operator>>(std::istream& stream, FileRequest& request)
		{
			stream >> *request.m_file;

			return stream;
		}
	}
}