//#include "../MemoryAllocation/Memory.h"
#include "StringList.h"
#include "StringManager.h"

namespace Ultrality
{
	namespace StringTools
	{
		StringList::StringList(const unsigned __int8& lengthOfList)
		{
			this->p_StringManager = StringManager::getInstance();
			this->m_stringList = Memory::allocateArray<LocalString>(this->p_StringManager->m_stringAllocatorForStringLists, lengthOfList);
			this->m_totalCapasity = lengthOfList;
		}
		StringList::StringList(const StringList& list)
		{
			this->p_StringManager = StringManager::getInstance();
			this->m_stringList = Memory::allocateArray<LocalString>(this->p_StringManager->m_stringAllocatorForStringLists, list.m_totalCapasity);
			this->m_totalCapasity = list.m_totalCapasity;
			for (unsigned __int8 i = 0; i < list.m_numOFStrings; i++)
			{
				this->m_stringList[i] = list.m_stringList[i];
			}
			this->m_numOFStrings = list.m_numOFStrings;
		}
		StringList::StringList(StringList&& list)
		{
			this->p_StringManager = StringManager::getInstance();
			this->m_stringList = list.m_stringList;
			list.m_stringList = nullptr;
			this->m_numOFStrings = list.m_numOFStrings;
			list.m_numOFStrings = 0;
			this->m_totalCapasity = list.m_totalCapasity;
			list.m_totalCapasity = 0;
		}

		StringList::StringList()
		{
			this->p_StringManager = StringManager::getInstance();
		}
		StringList::~StringList()
		{
			if (this->m_stringList)
			{
				/*if (this->m_numOFStrings > 0)
				{
					for (unsigned __int8 i = 0; i < this->m_numOFStrings; i++)
					{
						this->m_stringList[i].~LocalString();
					}
					this->m_numOFStrings = 0;
				}*/
				
				Memory::deallocateArray<LocalString>(this->p_StringManager->m_stringAllocatorForStringLists, this->m_stringList);
				this->m_stringList = nullptr;
			}
			this->m_totalCapasity = 0;
			this->p_StringManager = nullptr;
		}

		void StringList::AddString(LocalString&& string)
		{
			this->m_stringList[this->m_numOFStrings] = (LocalString&&)string;
			this->m_numOFStrings++;
		}

		void StringList::ClearList()
		{
			if (this->m_numOFStrings > 0 && this->m_stringList)
			{
				for (unsigned __int8 i = 0; i < this->m_numOFStrings; i++)
				{
					this->m_stringList[i].~LocalString();
				}

				this->m_numOFStrings = 0;
			}
		}

		void StringList::operator=(StringList& newList)
		{
			if (this->m_stringList)
				Memory::deallocateArray<LocalString>(this->p_StringManager->m_stringAllocatorForStringLists, this->m_stringList);
			this->m_stringList = Memory::allocateArray<LocalString>(this->p_StringManager->m_stringAllocatorForStringLists, newList.m_numOFStrings);
			for (unsigned __int8 i = 0; i < newList.m_numOFStrings; i++)
			{
				this->m_stringList[i] = newList.m_stringList[i];
			}

			this->m_totalCapasity, this->m_numOFStrings = newList.m_numOFStrings;
		}
		void StringList::operator=(StringList&& newList)
		{
			if (this->m_stringList)
				Memory::deallocateArray<LocalString>(this->p_StringManager->m_stringAllocatorForStringLists, this->m_stringList);
			this->m_stringList = newList.m_stringList;
			this->m_totalCapasity = newList.m_totalCapasity;
			this->m_numOFStrings = newList.m_numOFStrings;
			newList.m_stringList = nullptr;
		}

		std::ostream& operator<<(std::ostream& stream, const StringList& list)
		{
			for (unsigned __int8 i = 0; i < list.m_numOFStrings; i++)
			{
				if (i == 0)
					stream << list.m_stringList[i];
				else
					stream <<  ',' << list.m_stringList[i];
			}

			return stream;
		}
		std::istream& operator >> (std::istream& stream, StringList& list)
		{
			bool end = false;
			char templist[255] = {};

			stream.getline(list.p_StringManager->m_forMethodeAlloc, 65535);

			unsigned __int16 count = 0;
			unsigned __int8 tempcount = 0;

			while (!end)
			{
				while (list.p_StringManager->m_forMethodeAlloc[count] != ',' && list.p_StringManager->m_forMethodeAlloc[count] != '\0')
				{
					templist[tempcount] = list.p_StringManager->m_forMethodeAlloc[count];

					count++;
					tempcount++;
				}
				templist[tempcount] = NULL;
				if (list.p_StringManager->m_forMethodeAlloc[count] == '\0')
					end = true;
				else
					count++;

				list.p_StringManager->m_ListForFileReading->AddString(LocalString(templist));

				tempcount = 0;
			}

			list = *list.p_StringManager->m_ListForFileReading;

			list.p_StringManager->m_ListForFileReading->m_numOFStrings = 0;

			return stream;
		}
	}
}