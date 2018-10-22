#pragma once

#include "LocalString.h"
#include <iostream>

namespace Ultrality
{
	namespace StringTools
	{
		//class LocalString;

		class StringList
		{
		private:
			LocalString* m_stringList = nullptr;
			unsigned __int8 m_numOFStrings = 0;
			unsigned __int8 m_totalCapasity = 0;

			StringTools::StringManager* p_StringManager = nullptr;

		public:
			StringList(const unsigned __int8& lengthOfList);
			StringList(const StringList& list);
			StringList(StringList&& list);

			StringList();
			~StringList();

			//adds the specified string to the list
			void AddString(LocalString&& string);
			//deleats all of the strings in the list
			void ClearList();

			//will create a new array of LocalStrings and copy them from source (newList) to destination(this)
			//the created list will be the length of the source's m_NumOfStrings, not the total capasity
			void operator=(StringList& newList);
			//will take the array of the source (newList) and use it in the destination (this)
			//all other values will be taken as well
			void operator=(StringList&& newList);

			friend std::ostream& operator<<(std::ostream& stream, const StringList& list);
			friend std::istream& operator>>(std::istream& stream, StringList& list);
		};
	}
}