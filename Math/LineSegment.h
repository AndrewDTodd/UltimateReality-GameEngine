#pragma once
#include "Vector3.h"
#include "../GameObjects/GameObject.h"

namespace Ultrality
{
	namespace Math
	{
		class LineSegment : public GameObjects::GameObject
		{
		private:
			Vector3 pointOne;
			Vector3 pointTwo;
			Vector3 L;


		public:
			LineSegment(const Vector3& pointOne, const Vector3& pointTwo);

			Vector3 FindPointOnSegment(const float& distance);
		};
	}
}
