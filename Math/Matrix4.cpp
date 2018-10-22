#include "Matrix4.h"
#include <iostream>
#include <iomanip>

namespace Ultrality
{
	namespace Math
	{
		Matrix4::Matrix4()
		{
			
			this->m_Matrix = DirectX::XMMatrixIdentity();
		}
		Matrix4::Matrix4(const Vector4& row1, const Vector4& row2, const Vector4& row3, const Vector4& row4)
		{
			this->m_Matrix = DirectX::XMMATRIX(row1.m_Vector, row2.m_Vector, row3.m_Vector, row4.m_Vector);
		}
		Matrix4::Matrix4(const float& angle, const Type& type, const Axis& axis)
		{
			const float radians = Ultrality::Math::DegreesToRadians(angle);

			switch (type)
			{
			case Degrees:
				switch (axis)
				{
				case X:
					this->m_Matrix = DirectX::XMMatrixSet
					(
						1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, DirectX::XMScalarCos(radians), DirectX::XMScalarSin(radians), 0.0f,
						0.0f, -DirectX::XMScalarSin(radians), DirectX::XMScalarCos(radians), 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f
					);

				case Y:
					this->m_Matrix = DirectX::XMMatrixSet
					(
						DirectX::XMScalarCos(radians), 0.0f, -DirectX::XMScalarSin(radians), 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						DirectX::XMScalarSin(radians), 0.0f, DirectX::XMScalarCos(radians), 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f
					);

				case Z:
					this->m_Matrix = DirectX::XMMatrixSet
					(
						DirectX::XMScalarCos(radians), DirectX::XMScalarSin(radians), 0.0f, 0.0f,
						-DirectX::XMScalarSin(radians), DirectX::XMScalarCos(radians), 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f
					);
				}

			case Radians:
				switch (axis)
				{
				case X:
					this->m_Matrix = DirectX::XMMatrixSet
					(
						1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, DirectX::XMScalarCos(angle), DirectX::XMScalarSin(angle), 0.0f,
						0.0f, -DirectX::XMScalarSin(angle), DirectX::XMScalarCos(angle), 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f
					);

				case Y:
					this->m_Matrix = DirectX::XMMatrixSet
					(
						DirectX::XMScalarCos(angle), 0.0f, -DirectX::XMScalarSin(angle), 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						DirectX::XMScalarSin(angle), 0.0f, DirectX::XMScalarCos(angle), 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f
					);

				case Z:
					this->m_Matrix = DirectX::XMMatrixSet
					(
						DirectX::XMScalarCos(angle), DirectX::XMScalarSin(angle), 0.0f, 0.0f,
						-DirectX::XMScalarSin(angle), DirectX::XMScalarCos(angle), 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f
					);
				}
			}
		}
		Matrix4::Matrix4(const float& scaleX, const float& scaleY, const float& scaleZ)
		{
			this->m_Matrix = DirectX::XMMatrixScaling(scaleX, scaleY, scaleZ);
		}
		Matrix4::Matrix4(const float& unifiedScale)
		{
			this->m_Matrix = DirectX::XMMatrixScaling(unifiedScale, unifiedScale, unifiedScale);
		}
		Matrix4::Matrix4(const Quaternion& rotation)
		{
			this->m_Matrix = DirectX::XMMatrixRotationQuaternion(rotation.m_Vector);
		}
		Matrix4::Matrix4(DirectX::FXMVECTOR& v1, DirectX::FXMVECTOR& v2, DirectX::FXMVECTOR& v3, DirectX::FXMVECTOR& v4)
		{
			this->m_Matrix = DirectX::XMMATRIX(v1, v2, v3, v4);
		}
		Matrix4::Matrix4(DirectX::FXMMATRIX& matrix)
		{
			this->m_Matrix = matrix;
		}
		Matrix4::Matrix4(const Vector3& translation)
		{
			this->m_Matrix = DirectX::XMMatrixTranslationFromVector(translation.m_Vector);
		}
		Matrix4::Matrix4(DirectX::FXMVECTOR& translation)
		{
			this->m_Matrix = DirectX::XMMatrixTranslationFromVector(translation);
		}
		/*Matrix4::Matrix4(Matrix3& mat3)
		{
			this->matrix[0][0] = mat3.matrix[0][0]; this->matrix[0][1] = mat3.matrix[0][1]; this->matrix[0][2] = mat3.matrix[0][2]; this->matrix[0][3] = 0.0f;
			this->matrix[1][0] = mat3.matrix[1][0]; this->matrix[1][1] = mat3.matrix[1][1]; this->matrix[1][2] = mat3.matrix[1][2]; this->matrix[1][3] = 0.0f;
			this->matrix[2][0] = mat3.matrix[2][0]; this->matrix[2][1] = mat3.matrix[2][1]; this->matrix[2][2] = mat3.matrix[2][2]; this->matrix[2][3] = 0.0f;
			this->matrix[3][0] = 0.0f; this->matrix[3][1] = 0.0f; this->matrix[3][2] = 0.0f; this->matrix[3][3] = 1.0f;
		}*/

		Vector4 Matrix4::getRow1()
		{
			return Vector4(this->m_Matrix.r[0]);
		}
		Vector4 Matrix4::getRow2()
		{
			return Vector4(this->m_Matrix.r[1]);
		}
		Vector4 Matrix4::getRow3()
		{
			return Vector4(this->m_Matrix.r[2]);
		}
		Vector4 Matrix4::getRow4()
		{
			return Vector4(this->m_Matrix.r[3]);
		}

		Vector4 Matrix4::getCol1()
		{
			Matrix4 transpose = DirectX::XMMatrixTranspose(this->m_Matrix);

			return Vector4(transpose.m_Matrix.r[0]);
		}
		Vector4 Matrix4::getCol2()
		{
			Matrix4 transpose = DirectX::XMMatrixTranspose(this->m_Matrix);

			return Vector4(transpose.m_Matrix.r[1]);
		}
		Vector4 Matrix4::getCol3()
		{
			Matrix4 transpose = DirectX::XMMatrixTranspose(this->m_Matrix);

			return Vector4(transpose.m_Matrix.r[2]);
		}
		Vector4 Matrix4::getCol4()
		{
			Matrix4 transpose = DirectX::XMMatrixTranspose(this->m_Matrix);

			return Vector4(transpose.m_Matrix.r[3]);
		}

		float& Matrix4::getIndex(int row, int col)
		{
			DirectX::XMFLOAT4X4 mat;
			DirectX::XMStoreFloat4x4(&mat, this->m_Matrix);

			return mat.m[row][col];
		}

		std::ostream& operator<<(std::ostream& stream, Matrix4& matrix)
		{
			stream << "[" << std::fixed << std::setprecision(5) << matrix.getRow1() << "] \n";
			stream << "|" << std::fixed << std::setprecision(5) << matrix.getRow2() << "| \n";
			stream << "|" << std::fixed << std::setprecision(5) << matrix.getRow3() << "| \n";
			stream << "[" << std::fixed << std::setprecision(5) << matrix.getRow4() << "] \n";

			return stream;
		}

		/*Matrix4& Matrix4::Multiply(Matrix4& other)
		{
			
		}*/
		Matrix4 operator*(Matrix4& left, Matrix4& right)
		{
			return left.m_Matrix.operator*(right.m_Matrix);
		}
		Matrix4& Matrix4::operator*=(Matrix4& other)
		{
			this->m_Matrix.operator*=(other.m_Matrix);

			return *this;
		}

		/*Matrix4& Matrix4::DivideByScalar(float& scalar)
		{
			this->matrix[0][0] /= scalar; this->matrix[0][1] /= scalar; this->matrix[0][2] /= scalar; this->matrix[0][3] /= scalar;
			this->matrix[1][0] /= scalar; this->matrix[1][1] /= scalar; this->matrix[1][2] /= scalar; this->matrix[1][3] /= scalar;
			this->matrix[2][0] /= scalar; this->matrix[2][1] /= scalar; this->matrix[2][2] /= scalar; this->matrix[2][3] /= scalar;
			this->matrix[3][0] /= scalar; this->matrix[3][1] /= scalar; this->matrix[3][2] /= scalar; this->matrix[3][3] /= scalar;

			return *this;
		}*/
		Matrix4 operator/(Matrix4& left, float& scalar)
		{
			return left.m_Matrix.operator/(scalar);
		}
		Matrix4& Matrix4::operator/=(float& scalar)
		{
			this->m_Matrix.operator/=(scalar);

			return *this;
		}

		bool Matrix4::operator==(const Matrix4& other)
		{
			DirectX::XMFLOAT4 row1;
			DirectX::XMStoreFloat4(&row1, DirectX::XMVectorNearEqual(this->m_Matrix.r[0], other.m_Matrix.r[0], Vector4::epsilon.m_Vector));
			
			DirectX::XMFLOAT4 row2;
			DirectX::XMStoreFloat4(&row2, DirectX::XMVectorNearEqual(this->m_Matrix.r[1], other.m_Matrix.r[1], Vector4::epsilon.m_Vector));

			DirectX::XMFLOAT4 row3;
			DirectX::XMStoreFloat4(&row3, DirectX::XMVectorNearEqual(this->m_Matrix.r[2], other.m_Matrix.r[2], Vector4::epsilon.m_Vector));

			DirectX::XMFLOAT4 row4;
			DirectX::XMStoreFloat4(&row4, DirectX::XMVectorNearEqual(this->m_Matrix.r[3], other.m_Matrix.r[3], Vector4::epsilon.m_Vector));

			if
			(
				row1.x == 1 && row2.x == 1 && row3.x == 1 && row4.x == 1
			)
			{
				return true;
			}

			else
			{
				return false;
			}
		}
		/*const bool Matrix4::operator==(const Matrix4& matrix1, const Matrix4& matrix2)
		{
			DirectX::XMFLOAT4 row1;
			DirectX::XMStoreFloat4(&row1, DirectX::XMVectorNearEqual(matrix1.m_Matrix.r[0], matrix2.m_Matrix.r[0], Vector4::epsilon.m_Vector));
																	 						
			DirectX::XMFLOAT4 row2;									 						
			DirectX::XMStoreFloat4(&row2, DirectX::XMVectorNearEqual(matrix1.m_Matrix.r[1], matrix2.m_Matrix.r[1], Vector4::epsilon.m_Vector));
																	 						
			DirectX::XMFLOAT4 row3;									 						
			DirectX::XMStoreFloat4(&row3, DirectX::XMVectorNearEqual(matrix1.m_Matrix.r[2], matrix2.m_Matrix.r[2], Vector4::epsilon.m_Vector));
																	 						
			DirectX::XMFLOAT4 row4;									 						
			DirectX::XMStoreFloat4(&row4, DirectX::XMVectorNearEqual(matrix1.m_Matrix.r[3], matrix2.m_Matrix.r[3], Vector4::epsilon.m_Vector));

			if
			(
				row1.x == 1 && row2.x == 1 && row3.x == 1 && row4.x == 1
			)
			{
				return true;
			}

			else
			{
				return false;
			}
		}*/

		Vector4& Matrix4::MultiplyVector4(Vector4& vector)
		{
			vector.setX(Vector4::DotProduct(vector, this->getCol1()));
			vector.setY(Vector4::DotProduct(vector, this->getCol2()));
			vector.setZ(Vector4::DotProduct(vector, this->getCol3()));
			vector.setW(Vector4::DotProduct(vector, this->getCol4()));

			return vector;
		}
		Vector4 operator*(Vector4& left, Matrix4& right)
		{
			return right.MultiplyVector4(left);
		}
		Vector4 operator*(Matrix4& left, Vector4& right)
		{
			return left.MultiplyVector4(right);
		}
		Vector4& Matrix4::operator*=(Vector4& vector)
		{
			return MultiplyVector4(vector);
		}

		float Matrix4::Determinant()
		{
			DirectX::XMFLOAT4 det;
			DirectX::XMStoreFloat4(&det, DirectX::XMMatrixDeterminant(this->m_Matrix));

			return det.x;
		}
		/*Matrix4 Matrix4::Adjoint()
		{
			Matrix4 Adjoint = Matrix4::zero;

			Adjoint.matrix[0][0] = this->matrix[0][0]; Adjoint.matrix[0][1] = -this->matrix[0][1]; Adjoint.matrix[0][2] = this->matrix[0][2]; Adjoint.matrix[0][3] = -this->matrix[0][3];
			Adjoint.matrix[1][0] = -this->matrix[1][0]; Adjoint.matrix[1][1] = this->matrix[1][1]; Adjoint.matrix[1][2] = -this->matrix[1][2]; Adjoint.matrix[1][3] = this->matrix[1][3];
			Adjoint.matrix[2][0] = this->matrix[2][0]; Adjoint.matrix[2][1] = -this->matrix[2][1]; Adjoint.matrix[2][2] = this->matrix[2][2]; Adjoint.matrix[2][3] = -this->matrix[2][3];
			Adjoint.matrix[3][0] = -this->matrix[3][0]; Adjoint.matrix[3][1] = this->matrix[3][1]; Adjoint.matrix[3][2] = -this->matrix[3][2]; Adjoint.matrix[3][3] = this->matrix[3][3];

			Adjoint.Transpose();

			return Adjoint;
		}*/

		void Matrix4::Invert()
		{
			DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(this->m_Matrix);

			this->m_Matrix = DirectX::XMMatrixInverse(&det, this->m_Matrix);
		}
		Matrix4 Matrix4::Inverse()
		{
			DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(this->m_Matrix);

			return Matrix4(DirectX::XMMatrixInverse(&det, this->m_Matrix));
		}
		const Matrix4 Matrix4::Inverse(Matrix4& matrix)
		{
			DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(matrix.m_Matrix);

			return Matrix4(DirectX::XMMatrixInverse(&det, matrix.m_Matrix));
		}

		void Matrix4::Transpose()
		{
			this->m_Matrix = DirectX::XMMatrixTranspose(this->m_Matrix);
		}

		const Matrix4 Matrix4::Transposition(const Matrix4& matrix)
		{
			return Matrix4(DirectX::XMMatrixTranspose(matrix.m_Matrix));
		}

		const Matrix4 Matrix4::Identity(Vector4(1.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 0.0f), Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		const Matrix4 Matrix4::zero(Vector4(0.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 0.0f, 0.0f));
	}
}