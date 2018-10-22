#pragma once

#include "../../FileSystem.h"
#include "../../../StringTools/UltraString.h"
#include "../../../StringTools/StringTable.h"
#include <iostream>

namespace Ultrality
{
	namespace FileIO
	{
		class UltraText : public FileFormat
		{
		private:
			FileIO::FileManager* p_FileManager = nullptr;
		public:
			//StringTable that will hold the strings
			StringTools::StringTable* m_stringTable = nullptr;

			UltraText(StringTools::StringTable* table, StringTools::LocalString&& filename, StringTools::LocalString&& fileLocation);
			UltraText(StringTools::LocalString&& fileName, StringTools::LocalString&& fileLocation);

			~UltraText();

			std::ostream& write(std::ostream& stream);
			std::istream& read(std::istream& stream);
		};
	}
}
