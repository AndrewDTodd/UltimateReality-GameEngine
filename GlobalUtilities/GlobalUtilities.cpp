#include "GlobalUtilities.h"

namespace Ultrality
{
	namespace GlobUtil
	{
		void GlobalUtilities::Hasher(StringTools::UltraString* string)
		{
			if (sizeof(wchar_t) == 2)
			{
				for (int i = 0; i < string->m_stringLength - 2; i += 2)
				{
					string->m_stringHash += (unsigned __int32)((string->m_wcharArray[i] << 16) | (string->m_wcharArray[i + 1]));
				}
			}
			else
			{
				for (int i = 0; i < string->m_stringLength - 1; i++)
				{
					string->m_stringHash += (unsigned __int32)string->m_wcharArray[i];
				}
			}
		}

		unsigned __int32 GlobalUtilities::Hasher(float& value)
		{
			union
			{
				float m_asFloat;
				unsigned __int32 m_asU32;
			} u;

			u.m_asFloat = value;
			return u.m_asU32;
		}

		unsigned __int32 GlobalUtilities::Hasher(const StringTools::UltraString& string)
		{
			unsigned __int32 hash = 0;

			if (sizeof(wchar_t) == 2)
			{
				for (int i = 0; i < string.m_stringLength - 2; i += 2)
				{
					hash += (unsigned __int32)((string.m_wcharArray[i] << 16) | (string.m_wcharArray[i + 1] << 0));
				}
			}
			else
			{
				for (int i = 0; i < string.m_stringLength - 1; i++)
				{
					hash += (unsigned __int32)string.m_wcharArray[i];
				}
			}

			return hash;
		}

		unsigned __int32 GlobalUtilities::Hasher(char* charString)
		{
			unsigned __int32 hash = 0;

			for (int i = 0; charString[i] != NULL; i += 4)
			{
				hash += (unsigned __int32)((charString[i] << 24) | (charString[i + 1] << 16) | (charString[i + 2] << 8) | (charString[i + 3] << 0));
			}

			return hash;
		}
	}
}