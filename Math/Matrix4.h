#pragma once
#include <iostream>
#include "Vector4.h"
//#include "Matrix3.h"
#include "Quaternion.h"
#include <DirectXMath.h>

namespace Ultrality
{
	namespace Math
	{
		class Vector4;
		//class Matrix3;
		class Vector3;

		class Matrix4
		{
		private:
			DirectX::XMMATRIX m_Matrix;

		public:
			Matrix4();
			Matrix4(const Vector4& row1, const Vector4& row2, const Vector4& row3, const Vector4& row4);
			Matrix4(const float& angle, const Type& type, const Axis& axis);
			Matrix4(const float& scaleX, const float& scaleY, const float& scaleZ);
			Matrix4(const float& unifiedScale);
			Matrix4(const Quaternion& rotation);
			Matrix4(DirectX::FXMVECTOR& v1, DirectX::FXMVECTOR& v2, DirectX::FXMVECTOR& v3, DirectX::FXMVECTOR& v4);
			Matrix4(DirectX::FXMMATRIX& matrix);
			Matrix4(const Vector3& translation);
			Matrix4(DirectX::FXMVECTOR& translation);
			//Matrix4(Matrix3& mat3);

			Vector4 getRow1();
			Vector4 getRow2();
			Vector4 getRow3();
			Vector4 getRow4();

			Vector4 getCol1();
			Vector4 getCol2();
			Vector4 getCol3();
			Vector4 getCol4();

			float& getIndex(int row, int col);

			friend std::ostream& operator<<(std::ostream& stream, Matrix4& matrix);

			//Matrix4& Multiply(Matrix4& other);
			friend Matrix4 operator*(Matrix4& left, Matrix4& right);
			Matrix4& operator*=(Matrix4& other);

			//Matrix4& DivideByScalar(float& scalar);
			friend Matrix4 operator/(Matrix4& left, float& right);
			Matrix4& operator/=(float& scalar);

			Vector4& MultiplyVector4(Vector4& vector);
			friend Vector4 operator*(Vector4& left, Matrix4& right);
			friend Vector4 operator*(Matrix4& left, Vector4& right);
			Vector4& operator*=(Vector4& vector);

			bool operator==(const Matrix4& other);
			//static const bool operator==(const Matrix4& matrix1, const Matrix4& matrix2);

			float Determinant();
			//Matrix4 Adjoint();

			void Invert();
			Matrix4 Inverse();
			static const Matrix4 Inverse(Matrix4& matrix);

			void Transpose();
			static const Matrix4 Transposition(const Matrix4& matrix);

			static const Matrix4 Identity;
			static const Matrix4 zero;
		};
	}
}
