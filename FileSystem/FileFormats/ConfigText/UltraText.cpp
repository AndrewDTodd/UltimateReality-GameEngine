#include "UltraText.h"

namespace Ultrality
{
	namespace FileIO
	{
		UltraText::UltraText(StringTools::StringTable* table, StringTools::LocalString&& fileName, StringTools::LocalString&& fileLocation)
			:FileFormat((StringTools::LocalString&&)fileName, (StringTools::LocalString&&)fileLocation)
		{
			this->p_FileManager = FileManager::getInstance();
			this->m_fileExtension = StringTools::LocalString(".ULTXT");
			this->m_format = StorageFormat::BINARY;
			this->m_stringTable = table;
		}
		UltraText::UltraText(StringTools::LocalString&& fileName, StringTools::LocalString&& fileLocation)
			:FileFormat((StringTools::LocalString&&)fileName, (StringTools::LocalString&&)fileLocation)
		{
			this->p_FileManager = FileManager::getInstance();
			this->m_fileExtension = StringTools::LocalString(".ULTXT");
			this->m_format = StorageFormat::BINARY;
		}

		UltraText::~UltraText()
		{
			this->m_stringTable = nullptr;
		}

		std::ostream& UltraText::write(std::ostream& stream)
		{
			this->m_stringTable->write(stream);

			return stream;
		}
		std::istream& UltraText::read(std::istream& stream)
		{
			this->m_stringTable->read(stream);

			return stream;
		}
	}
}