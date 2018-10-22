#pragma once
#include <DirectXMath.h>

namespace Ultrality
{
	namespace Math
	{
		class Matrix2
		{
		private:
			DirectX::XMMATRIX m_Matrix;

		public:
			Matrix2();
			Matrix2(const Matrix2& matrix);
			Matrix2(const float[4]);

			float Determinate();

		};
	}
}