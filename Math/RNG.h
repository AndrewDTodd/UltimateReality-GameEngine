#pragma once


namespace Ultrality
{
	namespace Math
	{
		class Triple 
		{
			friend class RNG;

		private:
			unsigned __int8 m_a, m_b, m_c;

			Triple(const unsigned __int8& a, const unsigned __int8& b, const unsigned __int8& c) :
				m_a(a), m_b(b), m_c(c)
			{
			}
		};

		union Seed
		{
			friend class RNG;

		//private:
			unsigned __int64 m_intSeed = 1;
			unsigned __int32 m_int32Seed;

			Seed();
		};

		class RNG
		{
		private:
			static short mother1[10];
			static short mother2[10];
			static short mStart;

			#define m16Long 65536L				/* 2^16 */
			#define m16Mask 0xFFFF          /* mask for lower 16 bits */
			#define m15Mask 0x7FFF			/* mask for lower 15 bits */
			#define m31Mask 0x7FFFFFFF     /* mask for 31 bits */
			#define m32Double  4294967295.0  /* 2^32-1 */

			static unsigned __int8 m_tripleIndex;

			

			static const Triple m_tripleSets[275];
		public:

			static Seed m_seed;
			static const unsigned __int64& xor64();
			static const unsigned __int64& xor64Range(const unsigned __int64& tail, const unsigned __int64& head);

			static const double MOA32();
			//static const double MOA32Range(const __int32& tail, const __int32& head);
			//static const long double MOA64();
			//static const long double MOA64Range(const long double& tail, const long double& head);
		};
	}
}