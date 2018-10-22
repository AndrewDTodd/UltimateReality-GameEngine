#include "RNG.h"
#include <time.h>
#include <string.h>

namespace Ultrality
{
	namespace Math
	{
		short RNG::mStart = 1;

		short RNG::mother1[10] =
		{
			0,0,0,0,0,
			0,0,0,0,0
		};
		short RNG::mother2[10] =
		{
			0,0,0,0,0,
			0,0,0,0,0
		};

		Seed::Seed()
		{
			this->m_intSeed = 1;

			this->m_int32Seed = 1;
		}

		unsigned __int8 RNG::m_tripleIndex = 0;

		const Triple RNG::m_tripleSets[275] = 
		{
			Triple(1, 1, 54), Triple(1, 1, 55), Triple(1, 3, 45), Triple(1, 7, 9), Triple(1, 7, 44), Triple(1, 7, 46), Triple(1, 9, 50), Triple(1, 11, 35), Triple(1, 11, 50),
			Triple(1, 13, 45), Triple(1, 15, 4), Triple(1, 15, 63), Triple(1, 19, 6), Triple(1, 19, 16), Triple(1, 23, 14), Triple(1, 23, 29), Triple(1, 29, 34), Triple(1, 35, 5),
			Triple(1, 35, 11), Triple(1, 35, 34), Triple(1, 45, 37), Triple(1, 51, 13), Triple(1, 53, 3), Triple(1, 59, 14), Triple(2, 13, 23), Triple(2, 31, 51), Triple(2, 31, 53),
			Triple(2, 43, 27), Triple(2, 47, 49), Triple(3, 1, 11), Triple(3, 5, 21), Triple(3, 13, 59), Triple(3, 21, 31), Triple(3, 25, 20), Triple(3, 25, 31), Triple(3, 25, 56),
			Triple(3, 29, 40), Triple(3, 29, 47), Triple(3, 29, 49), Triple(3, 35, 14), Triple(3, 37, 17), Triple(3, 43, 4), Triple(3, 43, 6), Triple(3, 43, 11), Triple(3, 51, 16),
			Triple(3, 53, 7), Triple(3, 61, 17), Triple(3, 61, 26), Triple(4, 7, 19), Triple(4, 9, 13), Triple(4, 15, 51), Triple(4, 15, 53), Triple(4, 29, 45), Triple(4, 29, 49),
			Triple(4, 31, 33), Triple(4, 35, 15), Triple(4, 35, 21), Triple(4, 37, 11), Triple(4, 37, 21), Triple(4, 41, 19), Triple(4, 41, 45), Triple(4, 43, 21), Triple(4, 43, 31),
			Triple(4, 53, 7), Triple(5, 9, 23), Triple(5, 11, 54), Triple(5, 15, 27), Triple(5, 17, 11), Triple(5, 23, 36), Triple(5, 33, 29), Triple(5, 41, 20), Triple(5, 45, 16),
			Triple(5, 47, 23), Triple(5, 53, 20), Triple(5, 59, 33), Triple(5, 59, 35), Triple(5, 59, 63), Triple(6, 1, 17), Triple(6, 3, 49), Triple(6, 17, 47), Triple(6, 23, 27),
			Triple(6, 27, 7), Triple(6, 43, 21), Triple(6, 49, 29), Triple(6, 55, 17), Triple(7, 5, 41), Triple(7, 5, 47), Triple(7, 5, 55), Triple(7, 7, 20), Triple(7, 9, 38),
			Triple(7, 11, 10), Triple(7, 11, 35), Triple(7, 13, 58), Triple(7, 19, 17), Triple(7, 19, 54), Triple(7, 23, 8), Triple(7, 25, 58), Triple(7, 27, 59), Triple(7, 33, 8),
			Triple(7, 41, 40), Triple(7, 43, 28), Triple(7, 51, 24), Triple(7, 57, 12), Triple(8, 5, 59), Triple(8, 9, 25), Triple(8, 13, 25), Triple(8, 13, 61), Triple(8, 15, 21),
			Triple(8, 25, 59), Triple(8, 29, 19), Triple(8, 31, 17), Triple(8, 37, 21), Triple(8, 51, 21), Triple(9, 1, 27), Triple(9, 5, 36), Triple(9, 5, 43), Triple(9, 7, 18),
			Triple(9, 19, 18), Triple(9, 21, 11), Triple(9, 21, 20), Triple(9, 21, 40), Triple(9, 23, 57), Triple(9, 27, 10), Triple(9, 29, 12), Triple(9, 29, 37), Triple(9, 37, 31),
			Triple(9, 41, 45), Triple(10, 7, 33), Triple(10, 27, 59), Triple(10, 53, 13), Triple(11, 5, 32), Triple(11, 5, 34), Triple(11, 5, 43), Triple(11, 5, 45), Triple(11, 9, 14),
			Triple(11, 9, 34), Triple(11, 13, 40), Triple(11, 15, 37), Triple(11, 23, 42), Triple(11, 23, 56), Triple(11, 25, 48), Triple(11, 27, 26), Triple(11, 29, 14), Triple(11, 31, 18),
			Triple(11, 53, 23), Triple(12, 1, 31), Triple(12, 3, 13), Triple(12, 3, 49), Triple(12, 7, 13), Triple(12, 11, 47), Triple(12, 25, 27), Triple(12, 39, 49), Triple(12, 43, 19),
			Triple(13, 3, 40), Triple(13, 3, 53), Triple(13, 7, 17), Triple(13, 9, 15), Triple(13, 9, 50), Triple(13, 13, 19), Triple(13, 17, 43), Triple(13, 19, 28), Triple(13, 19, 47),
			Triple(13, 21, 18), Triple(13, 21, 49), Triple(13, 29, 35), Triple(13, 35, 30), Triple(13, 35, 38), Triple(13, 47, 23), Triple(13, 51, 21), Triple(14, 13, 17), Triple(14, 15, 19),
			Triple(14, 23, 33), Triple(14, 31, 45), Triple(14, 47, 15), Triple(15, 1, 19), Triple(15, 5, 37), Triple(15, 13, 28), Triple(15, 13, 52), Triple(15, 17, 27), Triple(15, 19, 63),
			Triple(15, 21, 46), Triple(15, 23, 23), Triple(15, 45, 17), Triple(15, 47, 16), Triple(15, 49, 26), Triple(16, 5, 17), Triple(16, 7, 39), Triple(16, 11, 19), Triple(16, 11, 27),
			Triple(16, 13, 55), Triple(16, 21, 35), Triple(16, 25, 43), Triple(16, 27, 53), Triple(16, 47, 17), Triple(17, 15, 58), Triple(17, 23, 29), Triple(17, 23, 51), Triple(17, 23, 52),
			Triple(17, 27, 22), Triple(17, 45, 22), Triple(17, 47, 28), Triple(17, 47, 29), Triple(17, 47, 54), Triple(18, 1, 25), Triple(18, 3, 43), Triple(18, 19, 19), Triple(18, 25, 21),
			Triple(18, 41, 23), Triple(19, 7, 36), Triple(19, 7, 55), Triple(19, 13, 37), Triple(19, 15, 46), Triple(19, 21, 52), Triple(19, 25, 20), Triple(19, 41, 21), Triple(19, 43, 27),
			Triple(20, 1, 31), Triple(20, 5, 29), Triple(21, 1, 27), Triple(21, 9, 29), Triple(21, 13, 52), Triple(21, 15, 28), Triple(21, 15, 29), Triple(21, 17, 24), Triple(21, 17, 30),
			Triple(21, 17, 48), Triple(21, 21, 32), Triple(21, 21, 34), Triple(21, 21, 37), Triple(21, 21, 38), Triple(21, 21, 40), Triple(21, 21, 41), Triple(21, 21, 43), Triple(21, 41, 23),
			Triple(22, 3, 39), Triple(23, 9, 38), Triple(23, 9, 48), Triple(23, 9, 57), Triple(23, 13, 38), Triple(23, 13, 58), Triple(23, 13, 61), Triple(23, 17, 25), Triple(23, 17, 54),
			Triple(23, 17, 56), Triple(23, 17, 62), Triple(23, 41, 34), Triple(23, 41, 51), Triple(24, 9, 35), Triple(24, 11, 29), Triple(24, 25, 25), Triple(24, 31, 35), Triple(25, 7, 46),
			Triple(25, 7, 49), Triple(25, 9, 39), Triple(25, 11, 57), Triple(25, 13, 29), Triple(25, 13, 39), Triple(25, 13, 62), Triple(25, 15, 47), Triple(25, 21, 44), Triple(25, 27, 27),
			Triple(25, 27, 53), Triple(25, 33, 36), Triple(25, 39, 54), Triple(28, 9, 55), Triple(28, 11, 53), Triple(29, 27, 37), Triple(31, 1, 51), Triple(31, 25, 37), Triple(31, 27, 35),
			Triple(33, 31, 43), Triple(33, 31, 55), Triple(43, 21, 46), Triple(49, 15, 61), Triple(55, 9, 56)
		}; 

		Seed RNG::m_seed = Seed::Seed();

		const unsigned __int64& RNG::xor64()
		{
			RNG::m_seed.m_intSeed = static_cast<__int64> (time(NULL));

			RNG::m_seed.m_intSeed = RNG::m_seed.m_intSeed * 1000;

			RNG::m_seed.m_intSeed = RNG::m_seed.m_intSeed ^ (RNG::m_seed.m_intSeed << RNG::m_tripleSets[RNG::m_tripleIndex].m_a); RNG::m_seed.m_intSeed = RNG::m_seed.m_intSeed ^ (RNG::m_seed.m_intSeed >> RNG::m_tripleSets[RNG::m_tripleIndex].m_b); (RNG::m_seed.m_intSeed = RNG::m_seed.m_intSeed ^ (RNG::m_seed.m_intSeed << RNG::m_tripleSets[RNG::m_tripleIndex].m_c));

			RNG::m_tripleIndex++;

			return RNG::m_seed.m_intSeed;
		}
		const unsigned __int64& RNG::xor64Range(const unsigned __int64& tail, const unsigned __int64& head)
		{
			RNG::m_seed.m_intSeed = static_cast<__int64> (time(NULL));

			RNG::m_seed.m_intSeed = RNG::m_seed.m_intSeed * 1000;

			RNG::m_seed.m_intSeed = RNG::m_seed.m_intSeed ^ (RNG::m_seed.m_intSeed << RNG::m_tripleSets[RNG::m_tripleIndex].m_a); RNG::m_seed.m_intSeed = RNG::m_seed.m_intSeed ^ (RNG::m_seed.m_intSeed >> RNG::m_tripleSets[RNG::m_tripleIndex].m_b); (RNG::m_seed.m_intSeed = RNG::m_seed.m_intSeed ^ (RNG::m_seed.m_intSeed << RNG::m_tripleSets[RNG::m_tripleIndex].m_c));

			RNG::m_tripleIndex++;

			return RNG::m_seed.m_intSeed = tail + (RNG::m_seed.m_intSeed % (head - tail + 1));
		}

		const double RNG::MOA32()
		{
			unsigned __int32* pSeed = &RNG::m_seed.m_int32Seed;
			*pSeed = static_cast<__int32> (time(NULL));

			unsigned long  number,
				number1,
				number2;
			short n,
				*p;
			unsigned short sNumber;

			/* Initialize motheri with 9 random values the first time */
			if (mStart) {
				sNumber = *pSeed&m16Mask;   /* The low 16 bits */
				number = *pSeed&m31Mask;   /* Only want 31 bits */

				p = mother1;
				for (n = 18; n--;) {
					number = 30903 * sNumber + (number >> 16);   /* One line
																 multiply-with-cary */
					*p++ = sNumber = number&m16Mask;
					if (n == 9)
						p = mother2;
				}
				/* make cary 15 bits */
				mother1[0] &= m15Mask;
				mother2[0] &= m15Mask;
				mStart = 0;
			}

			/* Move elements 1 to 8 to 2 to 9 */
			memmove(mother1 + 2, mother1 + 1, 8 * sizeof(short));
			memmove(mother2 + 2, mother2 + 1, 8 * sizeof(short));

			/* Put the carry values in numberi */
			number1 = mother1[0];
			number2 = mother2[0];

			/* Form the linear combinations */

			number1 += 1941 * mother1[2] + 1860 * mother1[3] + 1812 * mother1[4] + 1776 * mother1[5] +

				1492 * mother1[6] + 1215 * mother1[7] + 1066 * mother1[8] + 12013 * mother1[9];

			number2 += 1111 * mother2[2] + 2222 * mother2[3] + 3333 * mother2[4] + 4444 * mother2[5] +

				5555 * mother2[6] + 6666 * mother2[7] + 7777 * mother2[8] + 9272 * mother2[9];

			/* Save the high bits of numberi as the new carry */
			mother1[0] = static_cast<unsigned short>(number1) / m16Long;
			mother2[0] = static_cast<unsigned short>(number2) / m16Long;
			/* Put the low bits of numberi into motheri[1] */
			mother1[1] = m16Mask&number1;
			mother2[1] = m16Mask&number2;

			/* Combine the two 16 bit random numbers into one 32 bit */
			*pSeed = (((long)mother1[1]) << 16) + (long)mother2[1];

			/* Return a double value between 0 and 1 */
			return ((double)*pSeed) / m32Double;
		}
	}
}