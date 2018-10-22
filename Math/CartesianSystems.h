#pragma once
#include "Vector3.h"

namespace Ultrality
{
	namespace Math
	{
		const enum Axis
		{
			X,
			Y,
			Z
		};

		const enum Type
		{
			Radians,
			Degrees
		};

		float RadiansToDegrees(const float& radinas);
		float DegreesToRadians(const float& degrees);

		const double PI = 3.1415926535897932384626433832795;
	}
}