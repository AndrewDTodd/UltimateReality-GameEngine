#pragma once

#ifdef DEBUG
#include <assert.h>
#endif // DEBUG

//#include<string>
#include "../StringTools/UltraString.h"

#define MULTIPLY(a,b) (a*b)

namespace Ultrality
{
	namespace GlobUtil
	{
		class GlobalUtilities
		{
			friend class Ultrality::StringTools::UltraString;
		private:
			static void Hasher(StringTools::UltraString* string);
		public:
			static unsigned __int32 Hasher(float& value);
			static unsigned __int32 Hasher(const StringTools::UltraString& string);
			//use the tenplHasher function to Hash any 32bit object
			template<typename __32BitWord> static unsigned __int32 templHasher(__32BitWord& Word)
			{
#ifdef DEBUG
				assert(sizeof(__32BitWord) == 4);
#endif // DEBUG
				
				union
				{
					__32BitWord m_asQWord;
					unsigned __int32 m_asU32;
				} u;

				u.m_asQWord = Word;
				return u.m_asU32;
			}

			//use to Hash an array of char
			static unsigned __int32 Hasher(char* charString);
		};
	}
}