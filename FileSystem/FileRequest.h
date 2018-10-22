#pragma once

#include "FileFormats\FileFormat.h"

namespace Ultrality
{
	namespace FileIO
	{
		enum RequestType
		{
			WRITE,
			READ
		};
		enum RequestPriority
		{
			IMMEDIATE,
			EXTREAM,
			SEVERE,
			HIGH,
			MEDIUM,
			LOW
		};

		class FileRequest
		{
			friend class FileProcessor;
		private:
			//pointer to any "FileFormat" object, will be writen to, or from
			FileFormat* m_file = nullptr;
			//defines whether or not the request is for read or write
			RequestType m_requestType;
			//defines the priority that should be given to the request
			//ultimately determines where in the request queue the request will be issued
			RequestPriority m_requestPriority;

			StringTools::LocalString m_FileLocation;

			bool done = false;
		public:
			//will create the request, and otimaticaly issue it to be processed
			FileRequest(RequestType&& type, RequestPriority&& priority, FileFormat* file);
			~FileRequest();

			void WaitForFile();

			friend std::ostream& operator<<(std::ostream& stream, const FileRequest& request);
			friend std::istream& operator>>(std::istream& stream, FileRequest& request);
		};
	}
}