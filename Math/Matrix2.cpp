#include "Matrix2.h"

namespace Ultrality
{
	namespace Math
	{
		Matrix2::Matrix2()
		{
			this->m_Matrix.
		}
		Matrix2::Matrix2(const Matrix2& matrix)
		{
			this->matrix[0][0] = matrix.matrix[0][0]; this->matrix[0][1] = matrix.matrix[0][1];
			this->matrix[1][0] = matrix.matrix[1][0]; this->matrix[1][1] = matrix.matrix[1][1];
		}
		Matrix2::Matrix2(const float data[4])
		{
			this->matrix[0][0] = data[0]; this->matrix[0][1] = data[1];
			this->matrix[1][0] = data[2]; this->matrix[1][1] = data[3];
		}

		float Matrix2::Determinate()
		{
			return ((this->matrix[0][0] * this->matrix[1][1]) - (this->matrix[0][1] * this->matrix[1][0]));
		}
	}
}