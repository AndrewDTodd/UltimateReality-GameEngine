#include "LineSegment.h"

namespace Ultrality
{
	namespace Math
	{
		LineSegment::LineSegment(const Vector3& pointOne, const Vector3& pointTwo)
		{
			this->pointOne = pointOne;
			this->pointTwo = pointTwo;
			this->L = (pointTwo - pointOne);

			float mag = this->L.Magnitude();
			Vector3 normal = this->L.Normal();

			this->L = normal*mag;
		}

		Vector3 LineSegment::FindPointOnSegment(const float& distance)
		{
			if (distance <= 0.0f)
			{
				return this->pointOne;
			}

			if (distance >= 1.0f)
			{
				return this->pointTwo;
			}

			else
			{
				return (this->pointOne + (distance * this->L));
			}
		}
	}
}