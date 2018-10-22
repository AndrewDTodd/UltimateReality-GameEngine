#include "Sphere.h"

namespace Ultrality
{
	namespace Math
	{
		Sphere::Sphere(const Vector3& origin, const float& radius)
		{
			this->origin = origin;
			this->radius = radius;
		}
	}
}