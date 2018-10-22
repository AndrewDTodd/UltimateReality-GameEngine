#pragma once

#include <iostream>
#include "StringManager.h"

namespace Ultrality
{
	namespace StringTools
	{
		class UltraString
		{
			friend class StringTable;
		private:
			//converts an array of char from the locale encoding into an array of wchar(being UTF-16 in windows)
			inline void InputTowchar(const char* string);

			//copies a wchar array from "source" to this->m_wcharArray
			//"length" specifies the number of characters in the source array EXCUDING THE END OF STRING NULL
			inline void CopyString(const wchar_t* source, const unsigned __int16& length);

			StringManager* p_StringManager = nullptr;

			std::ostream& write(std::ostream& stream);
			std::istream& read(std::istream& stream);

		public:
			//array of wchar chars that constitutes the string(UTF-16 in windows)
			wchar_t* m_wcharArray = nullptr;
			//length of the UltraString's string in characters plus the end of string NULL
			unsigned __int16 m_stringLength = 0;
			//UltraStrings string hash, use to identify string by means of int operations
			unsigned __int32 m_stringHash = 0;

			//default constructor maintains listed values
			UltraString();
			//constructs an UltraString from a char array
			//use this for strings constructed by external input
			UltraString(const char* string);
			//constructs an UltraString from a wchar, stringLength is length of string EXCLUDING THE END OF STRING NULL
			//use this when construction strings internaly
			UltraString(const wchar_t* string, unsigned __int16& stringLength);
			//move constructor
			UltraString(UltraString&& string);
			//copy constructor
			UltraString(UltraString& string);

			~UltraString();

			//compairs one UltraString to another by means of the hash and stringLength, likely to produce acurate results
			//slight chance that it will return inacurate results
			//returns true if equal
			static const bool CompairByHash(const UltraString* one, const UltraString* two);

			//compairs one UltraString to another by means of the strings
			//longer operation, but will broduce 100% acurate results
			//first checks hashes to discard more intensive check if not applicable
			//returns true if equal
			static const bool CompairByStrings(const UltraString* one, const UltraString* two);

			//will return the number of char needed to hold the string if converted
			size_t NumOfCharNeededInBuffer();

			//converts the m_wcahrArray into a char array encoded in the locale encoding
			//pass in a charArray by pointer, and the number of char in the array
			//must pass in large enough buffer to hold converted string
			//returns the number of wchar converted
			size_t ConvertToLocaleCharArray(char* destination, size_t destinationCharCount);

			//constatanate the string with the "addition" specified
			void MovConstatanate(UltraString&& addition);
			void CpyConstatanate(UltraString& addition);

			//redefines the m_wcharArray into a new string
			void Redefine(char* newString);
			void Redefine(wchar_t* newString, unsigned __int16& length);

			//operator== for UltraString compairs two UltraStrings useing the CompairByHash methode
			//for 100% surtain compairisons use the CompairByString methode
			bool operator==(const UltraString& other);

			friend std::ostream& operator<<(std::ostream& stream, const UltraString& string);

			UltraString& operator+(UltraString& addition);
			UltraString& operator+(UltraString&& addition);

			UltraString& operator=(UltraString& newString);
			UltraString& operator=(UltraString&& newString);

			UltraString& operator+=(UltraString& addition);
			UltraString& operator+=(UltraString&& addition);
		};
	}
}
