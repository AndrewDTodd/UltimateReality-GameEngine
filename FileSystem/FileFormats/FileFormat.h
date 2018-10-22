#pragma once

#include "../../StringTools/LocalString.h"

namespace Ultrality
{
	namespace FileIO
	{
		class FileFormat
		{
			friend class FileProcessor;
		protected:
			FileFormat() = delete;
			FileFormat(FileFormat& file) = delete;
			FileFormat(FileFormat&& file) = delete;
		public:
			//enmumeration defining the two types of file storage formats, text, or binary
			enum StorageFormat
			{
				TEXT,
				BINARY
			};
			//values that will be defined per format type

			//string that defines the extension used to identify files of this format
			//will be used as "FileName{.Extension}"
			StringTools::LocalString m_fileExtension;
			//defines the formats storage form, text or binary
			//if format is binary, the data being represented in the file must be strings
			StorageFormat m_format;

			//values that are needed per file, despite format type

			//string used per file to store the name of the file
			StringTools::LocalString m_fileName;

			//string that stores the directory the file is in
			StringTools::LocalString m_fileLocation;

			//constructor
			FileFormat(StringTools::LocalString&& fileName, StringTools::LocalString&& fileLocation);
			virtual ~FileFormat();

			virtual std::ostream& write(std::ostream& stream) { return stream; };
			virtual std::istream& read(std::istream& stream) { return stream; };

			friend std::ostream& operator<<(std::ostream& stream, FileFormat& file);
			friend std::istream& operator>>(std::istream& stream, FileFormat& file);
		};
	}
}