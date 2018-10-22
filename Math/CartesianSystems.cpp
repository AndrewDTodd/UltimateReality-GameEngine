#include "CartesianSystems.h"

namespace Ultrality
{
	namespace Math
	{
		float RadiansToDegrees(const float& radians)
		{
			return (float) ((radians) * (180.0 / Ultrality::Math::PI));
		}

		float DegreesToRadians(const float& degrees)
		{
			return (float) ((degrees * Ultrality::Math::PI) / (180.0));
		}

		static const Vector3& i = Vector3::right;
		static const Vector3& j = Vector3::up;
		static const Vector3& k = Vector3::forward;
	}
}