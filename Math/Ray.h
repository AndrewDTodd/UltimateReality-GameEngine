#pragma once
#include "Vector3.h"
#include "../GameObjects/GameObject.h"

namespace Ultrality
{
	namespace Math
	{
		class Ray : public GameObjects::GameObject
		{
		private:
			Vector3 origin;
			Vector3 direction;

		public:
			Ray(const Vector3& origin, const Vector3& direction);

			Vector3 FindPointOnRay(const float& distance);
		};
	}
}
