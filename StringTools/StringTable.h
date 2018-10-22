#pragma once

#include "UltraString.h"
#include "../FileSystem/FileFormats/ConfigText/UltraText.h"
#include <iostream>

namespace Ultrality
{
	namespace FileIO
	{
		class UltraText;
	}
	namespace StringTools
	{
		//class UltraString;

		//StringTable that followes a global languages supported convention(number of columns per table)
		//represented in the static class member "cm_numOfLanguages"
		class StringTable
		{
			friend class Ultrality::FileIO::UltraText;
		private:
			//deleted copy constructor
			//copying is illegal
			StringTable(StringTable& toCopy) = delete;

			//deleted move construtor
			//moving is illegal
			StringTable(StringTable&& toMove) = delete;

			StringManager* p_StringManager = nullptr;

			std::ostream& write(std::ostream& stream);
			std::istream& read(std::istream& stream);

		public:
			//used to store the table of strings
			UltraString* m_stringTable = nullptr;
			//number of cells in table
			unsigned __int32 m_tableSize = 0;

			//holds the number of languages that are common to all standard tables
			//represents the number of columns in a standard table
			//default value is two, one for the string IDs, used internaly, second for default game language
			static unsigned __int8 cm_numOfLanguages;
			
			//default string table object will not contain any strings
			StringTable();
			//builds a standard table, supporting the number of languages specified in cm_numOfLanguages
			//number of columns in table is equal to the cm_numOfLanguages at time of construction
			//default is two
			//paramiter "NumOfRows" specifies the number of strings to be represented in the table
			StringTable(const unsigned __int32& NumOfRows);

			//destructor will clear the object
			~StringTable();

			//enteres the specified UltraString array into the table, based on its specified Identifier string
			//its index is determined by the Identifier strings ID % the numOfRows, collisions are not resolved
			//modification of the Identification string is nessisary to resolve colisions
			//the array must contain all desired entries, for new language entries to be made the row must be cleared
			//data will be moved, entered Strings will only persist in the table, they are not copied, they are moved
			//returns true if succeeded, false if failed
			bool EnterStrings(UltraString&& IdentificationString, UltraString* arrayOfLanguageOrderedStrings,const unsigned __int8& numOfLanguagesInArray);

			friend std::ostream& operator<<(std::ostream& stream, const StringTable& table);
		};
	}
}
