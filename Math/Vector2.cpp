#include "Vector2.h"

namespace Ultrality
{
	namespace Math
	{
		Vector2::Vector2()
		{
			vector.x = 0.0f;
			vector.y = 0.0f;
		}
		Vector2::Vector2(const float& x, const float& y)
		{
			vector.x = x;
			vector.y = y;
		}
	}
}