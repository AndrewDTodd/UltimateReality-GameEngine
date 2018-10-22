#pragma once
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

namespace Ultrality
{
	namespace Math
	{
		class Vector2
		{
		private:
			DirectX::XMFLOAT2 vector;

		public:
			Vector2();
			Vector2(const float& x, const float& y);
		};
	}
}

