#include "StringTable.h"
#include "StringManager.h"

namespace Ultrality
{
	namespace StringTools
	{
		unsigned __int8 StringTable::cm_numOfLanguages = 2;

		StringTable::StringTable()
		{
			this->p_StringManager = StringManager::getInstance();
		}
		StringTable::StringTable(const unsigned __int32& NumOfRows)
		{
			this->p_StringManager = StringManager::getInstance();
			this->m_stringTable = Memory::allocateArray<UltraString>(this->p_StringManager->StringTableAllocator, (NumOfRows * StringTable::cm_numOfLanguages));
			this->m_tableSize = NumOfRows * StringTable::cm_numOfLanguages;
		}

		StringTable::~StringTable()
		{
			if (this->m_stringTable)
			{
				Memory::deallocateArray<UltraString>(this->p_StringManager->StringTableAllocator, this->m_stringTable);
				this->m_stringTable = nullptr;
			}
		}

		bool StringTable::EnterStrings(UltraString&& IdentificationString, UltraString* arrayOfLanguageOrderedStrings,const unsigned __int8& numOfLanguagesInArray)
		{
			unsigned __int32 row = (IdentificationString.m_stringHash % this->m_tableSize);
			if (this->m_stringTable[row].m_stringHash == 0 && numOfLanguagesInArray <= StringTable::cm_numOfLanguages - 1 && row % StringTable::cm_numOfLanguages == 0)
			{
					this->m_stringTable[row] = static_cast<UltraString&&>(IdentificationString);
					IdentificationString.~UltraString();

					for (unsigned __int8 i = 0; i < numOfLanguagesInArray; i++)
					{
						this->m_stringTable[row + i + 1] = static_cast<UltraString&&>(arrayOfLanguageOrderedStrings[i]);
						arrayOfLanguageOrderedStrings[i].~UltraString();
					}
					return true;
			}
			else if (row % StringTable::cm_numOfLanguages != 0)
			{
				do
				{
					row--;
				} while (row % StringTable::cm_numOfLanguages != 0);

				if (this->m_stringTable[row].m_stringHash == 0)
				{
					this->m_stringTable[row] = static_cast<UltraString&&>(IdentificationString);
					IdentificationString.~UltraString();

					for (unsigned __int8 i = 0; i < numOfLanguagesInArray; i++)
					{
						this->m_stringTable[row + i + 1] = static_cast<UltraString&&>(arrayOfLanguageOrderedStrings[i]);
						arrayOfLanguageOrderedStrings[i].~UltraString();
					}
					return true;
				}
				else
				{
					return false;
				}
			}
			else
				return false;
		}

		std::ostream& StringTable::write(std::ostream& stream)
		{
			stream.write((char*)&this->m_tableSize, sizeof(unsigned __int32));
			for (unsigned __int32 i = 0; i < this->m_tableSize; i++)
			{
				this->m_stringTable[i].write(stream);
			}

			return stream;
		}
		std::istream& StringTable::read(std::istream& stream)
		{
			stream.read((char*)&this->m_tableSize, sizeof(unsigned __int32));
			if (this->m_stringTable)
				Memory::deallocateArray<UltraString>(this->p_StringManager->StringTableAllocator, this->m_stringTable);
			this->m_stringTable = Memory::allocateArray<UltraString>(this->p_StringManager->StringTableAllocator, this->m_tableSize);
			for (unsigned __int32 i = 0; i < this->m_tableSize; i++)
			{
				this->m_stringTable[i].read(stream);
			}

			return stream;
		}

		std::ostream& operator<<(std::ostream& stream, const StringTable& table)
		{
			if (table.m_tableSize > 0)
			{
				stream << table.m_stringTable[0] << ":";

				for (unsigned __int32 i = 1; i < table.m_tableSize; i++)
				{
					if (i % StringTable::cm_numOfLanguages == 0)
					{
						stream << "\n" << table.m_stringTable[i] << ":";
					}
					else
					{
						stream << table.m_stringTable[i] << ",";
					}
				}
			}
			else
			{
				stream << "This table is not initialized with any cells" << "\n";
			}

			return stream;
		}
	}
}