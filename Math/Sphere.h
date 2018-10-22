#pragma once
#include "Vector3.h"
#include "../GameObjects/GameObject.h"

namespace Ultrality
{
	namespace Math
	{
		class Sphere : public GameObjects::GameObject
		{
		private:
			Vector3 origin;
			float radius;

		public:
			Sphere(const Vector3& origin, const float& radius);
		};
	}
}
