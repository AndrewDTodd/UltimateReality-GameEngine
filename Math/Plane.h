#pragma once
#include "Vector4.h"
#include "../GameObjects/GameObject.h"

namespace Ultrality
{
	namespace Math
	{
		class Plane : public GameObjects::GameObject
		{
		private:
			Vector4 plane;

		public:
			Plane(Vector3& origin, Vector3& normal);
			Plane(const Vector4& plane);

			bool IsPointOnPlane(const Vector3& point);
		};
	}
}