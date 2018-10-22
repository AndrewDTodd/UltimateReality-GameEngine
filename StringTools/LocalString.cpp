#include "../MemoryAllocation/Memory.h"
#include "LocalString.h"

namespace Ultrality
{
	namespace StringTools
	{
		LocalString::LocalString(const char* string)
		{
			this->p_StringManager = StringManager::getInstance();

			unsigned __int16 i = 0;
			for (; string[i] != NULL; i++)
			{

			}

			this->m_charArray = Memory::allocateArray<char>(this->p_StringManager->LocalStringAllocator, i);

			for (unsigned __int16 j = 0; j <= i; j++)
			{
				this->m_charArray[j] = string[j];
			}

			this->m_stringLength = i;
		}

		LocalString::LocalString()
		{
			this->p_StringManager = StringManager::getInstance();
			this->m_charArray = nullptr;
			this->m_stringLength = 0;
		}
		LocalString::LocalString(LocalString&& string)
		{
			this->p_StringManager = StringManager::getInstance();

			this->m_charArray = string.m_charArray;
			this->m_stringLength = string.m_stringLength;

			string.m_charArray = nullptr;
			string.m_stringLength = 0;
			//string.~LocalString();
		}
		LocalString::LocalString(LocalString& string)
		{
			this->p_StringManager = StringManager::getInstance();
			this->m_charArray = Memory::allocateArray<char>(this->p_StringManager->LocalStringAllocator, string.m_stringLength);
			for (unsigned __int16 i = 0; i <= string.m_stringLength; i++)
			{
				this->m_charArray[i] = string.m_charArray[i];
			}
			this->m_stringLength = string.m_stringLength;
		}

		LocalString::~LocalString()
		{
			if (this->m_charArray)
			{
				this->m_mutex.lock();

				Memory::deallocateArray<char>(this->p_StringManager->LocalStringAllocator, this->m_charArray);
				this->m_charArray = nullptr;
				this->m_stringLength = 0;

				this->m_mutex.unlock();
			}
		}

		void LocalString::MovConstatanate(LocalString&& addition)
		{
			this->m_mutex.lock();
			char* oldString = this->m_charArray;

			this->m_charArray = Memory::allocateArray<char>(this->p_StringManager->LocalStringAllocator, (this->m_stringLength + (addition.m_stringLength)));

			unsigned __int16 i = 0;

			for (; i <= this->m_stringLength; i++)
			{
				this->p_StringManager->m_forMethodeAlloc[i] = oldString[i];
			}
			i--;

			Memory::deallocateArray<char>(this->p_StringManager->LocalStringAllocator, oldString);

			for (unsigned __int16 j = 0; j <= addition.m_stringLength; i++, j++)
			{
				this->p_StringManager->m_forMethodeAlloc[i] = addition.m_charArray[j];
			}

			oldString = nullptr;
			this->m_stringLength += addition.m_stringLength;
			this->m_mutex.unlock();
		}
		void LocalString::CpyConstatanate(LocalString& addition)
		{
			this->m_mutex.lock();
			char* oldString = this->m_charArray;

			this->m_charArray = Memory::allocateArray<char>(this->p_StringManager->LocalStringAllocator, (this->m_stringLength + addition.m_stringLength));

			unsigned __int16 i = 0;
			for (; i <= this->m_stringLength; i++)
			{
				this->m_charArray[i] = oldString[i];
			}
			i--;

			Memory::deallocateArray<char>(this->p_StringManager->LocalStringAllocator, oldString);

			for (unsigned __int16 j = 0; j <= addition.m_stringLength; i++, j++)
			{
				this->m_charArray[i] = addition.m_charArray[j];
			}

			oldString = nullptr;
			this->m_stringLength += addition.m_stringLength;
			this->m_mutex.unlock();
		}

		void LocalString::Redefine(const char* newString, unsigned __int16& length)
		{
			this->m_mutex.lock();
			Memory::deallocateArray<char>(this->p_StringManager->LocalStringAllocator, this->m_charArray);
			this->m_charArray = Memory::allocateArray<char>(this->p_StringManager->LocalStringAllocator, length);
			for (unsigned __int16 i = 0; i < length; i++)
			{
				this->m_charArray[i] = newString[i];
			}
			this->m_stringLength = length;
			this->m_mutex.unlock();
		}
		void LocalString::Redefine(const char* newString)
		{
			this->m_mutex.lock();
			Memory::deallocateArray<char>(this->p_StringManager->LocalStringAllocator, this->m_charArray);
			unsigned __int16 i = 0;
			for (; newString[i] != NULL; i++)
			{

			}

			this->m_charArray = Memory::allocateArray<char>(this->p_StringManager->LocalStringAllocator, i);

			this->m_stringLength = i;
			i = 0;

			for (; i <= this->m_stringLength; i++)
			{
				this->m_charArray[i] = newString[i];
			}
			this->m_mutex.unlock();
		}

		std::ostream& operator<<(std::ostream& stream, const LocalString& string)
		{
			for (unsigned __int16 i = 0; i < string.m_stringLength; i++)
			{
				stream << string.m_charArray[i];
			}

			return stream;
		}
		std::istream& operator>>(std::istream& stream, LocalString& string)
		{
			string.m_mutex.lock();
			stream.getline(string.p_StringManager->m_forMethodeAlloc, 65535);

			unsigned __int16 i = 0;
			for (; string.p_StringManager->m_forMethodeAlloc[i] != NULL; i++)
			{

			}

			if (string.m_charArray)
				Memory::deallocateArray<char>(string.p_StringManager->LocalStringAllocator, string.m_charArray);
			string.m_charArray = Memory::allocateArray<char>(string.p_StringManager->LocalStringAllocator, i);

			string.m_stringLength = i;
			i = 0;

			for (; i <= string.m_stringLength; i++)
			{
				string.m_charArray[i] = string.p_StringManager->m_forMethodeAlloc[i];
			}
			
			string.m_mutex.unlock();

			return stream;
		}

		LocalString& LocalString::operator+(LocalString& addition)
		{
			this->m_mutex.lock();
			unsigned __int16 i = 0;
			for (; i <= this->m_stringLength; i++)
			{
				this->p_StringManager->m_forMethodeAlloc[i] = this->m_charArray[i];
			}
			i--;

			for (unsigned __int16 j = 0; j <= addition.m_stringLength; j++, i++)
			{
				this->p_StringManager->m_forMethodeAlloc[i] = addition.m_charArray[j];
			}

			this->p_StringManager->m_forMethodeAlloc[i++] = NULL;

			static LocalString newString(this->p_StringManager->m_forMethodeAlloc);

			this->m_mutex.unlock();

			return newString;
		}
		LocalString& LocalString::operator+(LocalString&& addition)
		{
			this->m_mutex.lock();
			unsigned __int16 i = 0;
			for (; i <= this->m_stringLength; i++)
			{
				this->p_StringManager->m_forMethodeAlloc[i] = this->m_charArray[i];
			}
			i--;

			for (unsigned __int16 j = 0; j <= addition.m_stringLength; j++, i++)
			{
				this->p_StringManager->m_forMethodeAlloc[i] = addition.m_charArray[j];
			}

			this->p_StringManager->m_forMethodeAlloc[i++] = NULL;

			static LocalString newString(this->p_StringManager->m_forMethodeAlloc);

			this->m_mutex.unlock();

			return newString;
		}

		void LocalString::operator=(LocalString& newString)
		{
			this->m_mutex.lock();
			unsigned __int16 i = 0;
			if (this->m_stringLength == newString.m_stringLength)
			{
				for (; i <= this->m_stringLength; i++)
				{
					this->m_charArray[i] = newString.m_charArray[i];
				}
			}
			else
			{
				if(this->m_charArray)
					Memory::deallocateArray<char>(this->p_StringManager->LocalStringAllocator, this->m_charArray);
				this->m_charArray = Memory::allocateArray<char>(this->p_StringManager->LocalStringAllocator, newString.m_stringLength);

				for (; i <= newString.m_stringLength; i++)
				{
					this->m_charArray[i] = newString.m_charArray[i];
				}
				this->m_stringLength = newString.m_stringLength;
			}
			this->m_mutex.unlock();
		}
		void LocalString::operator=(LocalString&& newString)
		{
			this->m_mutex.lock();
			if(this->m_charArray)
				Memory::deallocateArray<char>(this->p_StringManager->LocalStringAllocator, this->m_charArray);
			this->m_charArray = newString.m_charArray;
			newString.m_charArray = nullptr;
			this->m_stringLength = newString.m_stringLength;
			newString.m_stringLength = 0;
			this->m_mutex.unlock();
		}
		void LocalString::operator=(char* string)
		{
			this->m_mutex.lock();
			unsigned __int16 i = 0;
			for (; string[i] != NULL; i++)
			{

			}

			if (this->m_stringLength == i)
			{
				i = 0;
				for (; i <= this->m_stringLength; i++)
				{
					this->m_charArray[i] = string[i];
				}
			}
			else
			{
				if(this->m_charArray)
					Memory::deallocateArray<char>(this->p_StringManager->LocalStringAllocator, this->m_charArray);
				this->m_charArray = Memory::allocateArray<char>(this->p_StringManager->LocalStringAllocator, i);
				
				this->m_stringLength = i;
				i = 0;

				for (; i <= this->m_stringLength; i++)
				{
					this->m_charArray[i] = string[i];
				}
			}
			this->m_mutex.unlock();
		}

		void LocalString::operator+=(LocalString& addition)
		{
			this->m_mutex.lock();
			char* oldString = this->m_charArray;

			this->m_charArray = Memory::allocateArray<char>(this->p_StringManager->LocalStringAllocator, (this->m_stringLength + addition.m_stringLength));

			unsigned __int16 i = 0;
			for (; i <= this->m_stringLength; i++)
			{
				this->m_charArray[i] = oldString[i];
			}
			i--;

			Memory::deallocateArray<char>(this->p_StringManager->LocalStringAllocator, oldString);

			for (unsigned __int16 j = 0; j <= addition.m_stringLength; i++, j++)
			{
				this->m_charArray[i] = addition.m_charArray[j];
			}

			oldString = nullptr;
			this->m_stringLength += addition.m_stringLength;
			this->m_mutex.unlock();
		}
		void LocalString::operator+=(LocalString&& addition)
		{
			this->m_mutex.lock();
			char* oldString = this->m_charArray;

			this->m_charArray = Memory::allocateArray<char>(this->p_StringManager->LocalStringAllocator, (this->m_stringLength + addition.m_stringLength));

			unsigned __int16 i = 0;
			for (; i <= this->m_stringLength; i++)
			{
				this->m_charArray[i] = oldString[i];
			}
			i--;

			Memory::deallocateArray<char>(this->p_StringManager->LocalStringAllocator, oldString);

			for (unsigned __int16 j = 0; j <= addition.m_stringLength; i++, j++)
			{
				this->m_charArray[i] = addition.m_charArray[j];
			}

			oldString = nullptr;
			this->m_stringLength += addition.m_stringLength;
			this->m_mutex.unlock();
		}

		void LocalString::endLine()
		{
			this->m_mutex.lock();
			char* oldString = this->m_charArray;

			this->m_charArray = Memory::allocateArray<char>(this->p_StringManager->LocalStringAllocator, (this->m_stringLength + 1));

			for (unsigned __int16 i = 0; i <= this->m_stringLength; i++)
			{
				this->m_charArray[i] = oldString[i];
			}

			Memory::deallocateArray<char>(this->p_StringManager->LocalStringAllocator, oldString);
			oldString = nullptr;

			this->m_charArray[this->m_stringLength] = NULL;
			this->m_mutex.unlock();
		}
	}
}