#include "Plane.h"

namespace Ultrality
{
	namespace Math
	{
		Plane::Plane(Vector3& origin, Vector3& normal)
		{
			normal.Normalize();

			this->plane = Vector4(normal.X(), normal.Y(), normal.Z(), origin.Magnitude());
		}
		Plane::Plane(const Vector4& plane)
		{
			this->plane = plane;
		}

		bool Plane::IsPointOnPlane(const Vector3& point)
		{
			return (Vector3::DotProduct(Vector3(this->plane.X(), this->plane.Y(), this->plane.Z()), point) == -this->plane.W());
		}
	}
}