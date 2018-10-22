#pragma once
#include "Vector3.h"
#include "../GameObjects/GameObject.h"

namespace Ultrality
{
	namespace Math
	{
		class GameObject;

		class Line : public GameObjects::GameObject
		{
		private:
			Vector3 origin;
			Vector3 direction;

		public:
			Line(const Vector3& origin, Vector3& direction);

			Vector3 FindPointOnLine(const float& distance);

		};
	}
}