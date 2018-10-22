#pragma once

#include <iostream>
#include "StringManager.h"
#include <mutex>

namespace Ultrality
{
	namespace StringTools
	{
		//class StringManager;
		class LocalString
		{
		private:
			//copies a char array from "source" to this->m_charArray
			//"length" specifies the number of characters in the source array EXCUDING THE END OF STRING NULL
			//inline void CopyString(const char* source, const unsigned __int16& length);

			StringTools::StringManager* p_StringManager = nullptr;
			std::recursive_mutex m_mutex;

		public:
			//array of chars that constitutes the string
			char* m_charArray = nullptr;
			//length of the LocalString's string in characters plus the end of string NULL
			unsigned __int16 m_stringLength = 0;

			LocalString();


			//constructs a LocalString from a char array
			//use this for strings constructed by external input
			LocalString(const char* string);
			//move constructor
			LocalString(LocalString&& string);
			//copy constructor
			LocalString(LocalString& string);

			~LocalString();

			//constatanate the string with the "addition" specified
			void MovConstatanate(LocalString&& addition);
			void CpyConstatanate(LocalString& addition);

			//redefines the m_charArray into a new string, "length" is the number of char in the array
			void Redefine(const char* newString, unsigned __int16& length);
			//redefines the m_charArray into the newString, self deturmines length
			void Redefine(const char* newString);

			friend std::ostream& operator<<(std::ostream& stream, const LocalString& string);
			friend std::istream& operator>>(std::istream& stream, LocalString& string);

			LocalString& operator+(LocalString& addition);
			LocalString& operator+(LocalString&& addition);

			void operator=(LocalString& newString);
			void operator=(LocalString&& newString);
			void operator=(char* string);

			void operator+=(LocalString& addition);
			void operator+=(LocalString&& addition);

			void endLine();
		};
	}
}
