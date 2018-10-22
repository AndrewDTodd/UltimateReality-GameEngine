#include "Line.h"

namespace Ultrality
{
	namespace Math
	{
		Line::Line(const Vector3& origin, Vector3& direction)
		{
			this->origin = origin;
			this->direction = direction.Normal();
		}

		Vector3 Line::FindPointOnLine(const float& distance)
		{
			return (this->origin + (distance * this->direction));
		}
	}
}