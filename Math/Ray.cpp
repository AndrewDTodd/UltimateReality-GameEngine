#include "Ray.h"

namespace Ultrality
{
	namespace Math
	{
		Ray::Ray(const Vector3& origin, const Vector3& direction)
		{
			this->origin = origin;
			this->direction = direction;
		}

		Vector3 Ray::FindPointOnRay(const float& distance)
		{
			return (this->origin + (+distance * this->direction));
		}
	}
}