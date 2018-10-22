#pragma once
#include "Vector3.h"
#include "../GameObjects/GameObject.h"

namespace Ultrality
{
	namespace Math
	{
		class AABB : public GameObjects::GameObject
		{
		private:
			Vector3 min;
			Vector3 max;

		public:
			AABB(const Vector3& min, const Vector3& max);

			bool IsPointInsideBox(const Vector3& point);
		};
	}
}
