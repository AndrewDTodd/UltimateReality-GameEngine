#define _CRT_SECURE_NO_WARNINGS
#include "../MemoryAllocation/Memory.h"
#include "UltraString.h"
#include "StringManager.h"

namespace Ultrality
{
	namespace StringTools
	{
		void UltraString::InputTowchar(const char* string)
		{
			size_t sizeReq = mbstowcs(nullptr, string, 0) + 1;

			this->m_wcharArray = Memory::allocateArray<wchar_t>(this->p_StringManager->UltraStringAllocator, sizeReq);

#ifdef Release
			mbstowcs(this->m_wcharArray, string, sizeReq);

			this->m_stringLength = sizeReq;
#endif // Release

#ifdef DEBUG
			size_t converted = mbstowcs(this->m_wcharArray, string, sizeReq) + 1;

			assert(converted == sizeReq);
			assert(sizeof(*this->m_wcharArray) == sizeof(wchar_t));

			this->m_stringLength = (unsigned __int16)converted;
#endif // DEBUG

			GlobUtil::GlobalUtilities::Hasher(this);

#ifdef DEBUG
			assert(this->m_stringHash != 0);
#endif // DEBUG

		}

		void UltraString::CopyString(const wchar_t* source, const unsigned __int16& length)
		{
			if (this->m_wcharArray)
			{
				Memory::deallocateArray<wchar_t>(this->p_StringManager->UltraStringAllocator, this->m_wcharArray);
				this->m_stringHash = 0;
				this->m_stringLength = 0;
			}

			Memory::allocateArray<wchar_t>(this->p_StringManager->UltraStringAllocator, length + 1);

			for (int i = 0; i <= length; i++)
			{
				this->m_wcharArray[i] = source[i];
			}
			this->m_wcharArray[length + 1] = NULL;

			this->m_stringLength = length + 1;
		}

		UltraString::UltraString()
		{
			this->p_StringManager = StringManager::getInstance();
		}
		UltraString::UltraString(const char* string)
		{
			this->p_StringManager = StringManager::getInstance();
			this->InputTowchar(string);
		}
		UltraString::UltraString(const wchar_t* string, unsigned __int16& stringLength)
		{
			this->p_StringManager = StringManager::getInstance();
			this->CopyString(string, stringLength);

			GlobUtil::GlobalUtilities::Hasher(this);
		}
		UltraString::UltraString(UltraString&& string)
			:m_wcharArray(string.m_wcharArray),
			m_stringLength(string.m_stringLength),
			m_stringHash(string.m_stringHash)
		{
			this->p_StringManager = StringManager::getInstance();
			//Memory::deallocateArray<wchar_t>(UltraString::UltraStringAllocator, string.m_wcharArray);
			string.m_wcharArray = nullptr;
			string.m_stringLength = 0;
			string.m_stringHash = 0;
		}
		UltraString::UltraString(UltraString& string)
		{
			this->p_StringManager = StringManager::getInstance();
			this->CopyString(string.m_wcharArray, string.m_stringLength - 1);
			this->m_stringHash = string.m_stringHash;
		}

		UltraString::~UltraString()
		{
			if (this->m_wcharArray)
			{
				Memory::deallocateArray<wchar_t>(this->p_StringManager->UltraStringAllocator, this->m_wcharArray);
				this->m_wcharArray = nullptr;
				this->m_stringLength = 0;
				this->m_stringHash = 0;
			}
		}

		const bool UltraString::CompairByHash(const UltraString* one, const UltraString* two)
		{
			return (one->m_stringHash == two->m_stringHash && one->m_stringLength == two->m_stringLength);
		}

		const bool UltraString::CompairByStrings(const UltraString* one, const UltraString* two)
		{
			if (one->m_stringHash != two->m_stringHash)
			{
				return false;
			}
			else
			{
				if (one->m_stringLength == two->m_stringLength)
				{
					for (unsigned __int16 i = 0; i <= one->m_stringLength; i++)
					{
						if (one->m_wcharArray[i] != two->m_wcharArray[i])
						{
							return false;
						}
					}
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		size_t UltraString::NumOfCharNeededInBuffer()
		{
			return wcstombs(NULL, this->m_wcharArray, 0) + 1;
		}

		size_t UltraString::ConvertToLocaleCharArray(char* destination, size_t destinationCharCount)
		{
			return wcstombs(destination, this->m_wcharArray, destinationCharCount);
		}

		void UltraString::MovConstatanate(UltraString&& addition)
		{
			wchar_t* oldString = this->m_wcharArray;

			this->m_wcharArray = Memory::allocateArray<wchar_t>(this->p_StringManager->UltraStringAllocator, (this->m_stringLength + (addition.m_stringLength - 1)));

			memcpy(this->m_wcharArray, oldString, ((this->m_stringLength - 1) * sizeof(wchar_t)));

			wchar_t* start = this->m_wcharArray + this->m_stringLength - 1;

			*start = *addition.m_wcharArray;

			oldString = nullptr;
			addition.m_wcharArray = 0;
			this->m_stringLength += addition.m_stringLength - 1;
			GlobUtil::GlobalUtilities::Hasher(this);
		}
		void UltraString::CpyConstatanate(UltraString& addition)
		{
			wchar_t* oldString = this->m_wcharArray;

			this->m_wcharArray = Memory::allocateArray<wchar_t>(this->p_StringManager->UltraStringAllocator, (this->m_stringLength + (addition.m_stringLength - 1)));

			memcpy(this->m_wcharArray, oldString, ((this->m_stringLength - 1) * sizeof(wchar_t)));

			wchar_t* start = this->m_wcharArray + this->m_stringLength - 1;

			memcpy(start, addition.m_wcharArray, ((addition.m_stringLength) * sizeof(wchar_t)));

			oldString = nullptr;
			this->m_stringLength += addition.m_stringLength - 1;
			GlobUtil::GlobalUtilities::Hasher(this);
		}

		void UltraString::Redefine(char* newString)
		{
			Memory::deallocateArray<wchar_t>(this->p_StringManager->UltraStringAllocator, this->m_wcharArray);

			this->m_stringHash = 0;
			this->m_stringLength = 0;
			this->m_wcharArray = nullptr;

			this->InputTowchar(newString);
		}
		void UltraString::Redefine(wchar_t* newString, unsigned __int16& length)
		{
			this->CopyString(newString, length - 1);
		}

		bool UltraString::operator==(const UltraString& other)
		{
			return UltraString::CompairByHash(this, &other);
		}

		std::ostream& operator<<(std::ostream& stream, const UltraString& string)
		{
			char* convertedArray = nullptr;
			size_t requiredSize = wcstombs(NULL, string.m_wcharArray, 0) + sizeof(char);

			convertedArray = Memory::allocateArray<char>(string.p_StringManager->UltraStringAllocator, (requiredSize / sizeof(char)));

			wcstombs(convertedArray, string.m_wcharArray, requiredSize);

			for (unsigned __int16 i = 0; i < requiredSize; i++)
			{
				stream << convertedArray[i];
			}

			Memory::deallocateArray<char>(string.p_StringManager->UltraStringAllocator, convertedArray);

			convertedArray = nullptr;

			return stream;
		}

		UltraString& UltraString::operator+(UltraString& addition)
		{
			wchar_t* oldString = this->m_wcharArray;

			this->m_wcharArray = Memory::allocateArray<wchar_t>(this->p_StringManager->UltraStringAllocator, (this->m_stringLength + (addition.m_stringLength - 1)));

			memcpy(this->m_wcharArray, oldString, ((this->m_stringLength - 1) * sizeof(wchar_t)));

			wchar_t* start = this->m_wcharArray + this->m_stringLength - 1;

			memcpy(start, addition.m_wcharArray, ((addition.m_stringLength) * sizeof(wchar_t)));

			oldString = nullptr;
			this->m_stringLength += addition.m_stringLength - 1;
			GlobUtil::GlobalUtilities::Hasher(this);

			return *this;
		}
		UltraString& UltraString::operator+(UltraString&& addition)
		{
			wchar_t* oldString = this->m_wcharArray;

			this->m_wcharArray = Memory::allocateArray<wchar_t>(this->p_StringManager->UltraStringAllocator, (this->m_stringLength + (addition.m_stringLength - 1)));

			memcpy(this->m_wcharArray, oldString, ((this->m_stringLength - 1) * sizeof(wchar_t)));

			wchar_t* start = this->m_wcharArray + this->m_stringLength - 1;

			*start = *addition.m_wcharArray;

			oldString = nullptr;
			addition.m_wcharArray = 0;
			this->m_stringLength += addition.m_stringLength - 1;
			GlobUtil::GlobalUtilities::Hasher(this);

			return *this;
		}

		UltraString& UltraString::operator=(UltraString& newString)
		{
			*this = UltraString(newString);
			return *this;
		}
		UltraString& UltraString::operator=(UltraString&& newString)
		{
			this->m_wcharArray = newString.m_wcharArray;
			this->m_stringLength = newString.m_stringLength;
			this->m_stringHash = newString.m_stringHash;

			newString.m_wcharArray = nullptr;
			newString.m_stringHash = 0;
			newString.m_stringLength = 0;
			
			return *this;
		}

		UltraString& UltraString::operator+=(UltraString& addition)
		{
			this->CpyConstatanate(addition);
			return *this;
		}
		UltraString& UltraString::operator+=(UltraString&& addition)
		{
			wchar_t* oldString = this->m_wcharArray;

			this->m_wcharArray = Memory::allocateArray<wchar_t>(this->p_StringManager->UltraStringAllocator, (this->m_stringLength + (addition.m_stringLength - 1)));

			memcpy(this->m_wcharArray, oldString, ((this->m_stringLength - 1) * sizeof(wchar_t)));

			wchar_t* start = this->m_wcharArray + this->m_stringLength - 1;

			*start = *addition.m_wcharArray;

			oldString = nullptr;
			addition.m_wcharArray = 0;
			this->m_stringLength += addition.m_stringLength - 1;
			GlobUtil::GlobalUtilities::Hasher(this);

			return *this;
		}

		std::ostream& UltraString::write(std::ostream& stream)
		{
			if (this->m_wcharArray)
			{
				stream.write((char*)&this->m_stringHash, sizeof(unsigned __int32));
				stream.write((char*)&this->m_stringLength, sizeof(unsigned __int16));
				stream.write((char*)this->m_wcharArray, (sizeof(wchar_t) * this->m_stringLength));
			}

			return stream;
		}
		std::istream& UltraString::read(std::istream& stream)
		{
			stream.read((char*)&this->m_stringHash, sizeof(unsigned __int32));
			stream.read((char*)&this->m_stringLength, sizeof(unsigned __int16));
			if (this->m_wcharArray)
				Memory::deallocateArray<wchar_t>(this->p_StringManager->UltraStringAllocator, this->m_wcharArray);
			this->m_wcharArray = Memory::allocateArray<wchar_t>(this->p_StringManager->UltraStringAllocator, this->m_stringLength);
			stream.read((char*)this->m_wcharArray, (sizeof(wchar_t) * this->m_stringLength));

			return stream;
		}
	}
}