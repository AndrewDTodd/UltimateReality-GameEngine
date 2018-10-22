#include "FileFormat.h"

namespace Ultrality
{
	namespace FileIO
	{
		FileFormat::FileFormat(StringTools::LocalString&& fileName, StringTools::LocalString&& fileLocation)
		{
			this->m_fileName = (StringTools::LocalString&&)fileName;
			this->m_fileLocation = (StringTools::LocalString&&)fileLocation;
		}
		FileFormat::~FileFormat()
		{
			this->m_fileLocation.~LocalString();
			this->m_fileName.~LocalString();
			this->m_fileExtension.~LocalString();
		}

		std::ostream& operator<<(std::ostream& stream, FileFormat& file)
		{
			return file.write(stream);
		}
		std::istream& operator>>(std::istream& stream, FileFormat& file)
		{
			return file.read(stream);
		}
	}
}