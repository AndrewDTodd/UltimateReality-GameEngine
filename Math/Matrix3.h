#pragma once
#include "Vector3.h"
#include <iostream>
#include "CartesianSystems.h"
#include "Quaternion.h"
#include "Matrix2.h"
#include <DirectXMath.h>

namespace Ultrality
{
	namespace Math
	{
		class Vector3;
		class Quaternion;
		enum Type;
		enum Axis;

		class Matrix3
		{
			friend class Matrix4;

		private:
			DirectX::XMMATRIX m_Matrix;

		public:
			Matrix3();
			Matrix3(Vector3& row1, Vector3& row2, Vector3& row3);
			Matrix3(const float& angle, const Type& type, const Axis& axis);
			Matrix3(const float& scaleX = 1.0f, const float& scaleY = 1.0f, const float& scaleZ = 1.0f);
			Matrix3(Quaternion& rotation);

			Vector3 getRow1();
			Vector3 getRow2();
			Vector3 getRow3();
			 
			Vector3 getCol1();
			Vector3 getCol2();
			Vector3 getCol3();

			float& getIndex(int row, int col);

			friend std::ostream& operator<<(std::ostream& stream, Matrix3& matrix);

			Matrix3& Multiply(Matrix3& other);
			friend Matrix3 operator*(Matrix3& left, Matrix3& right);
			Matrix3& operator*=(Matrix3& other);

			Matrix3& DivideByScalar(const float& scalar);
			friend Matrix3 operator/(Matrix3& left, float& right);
			Matrix3& operator/=(float& scalar);

			Vector3& MultiplyVector3(Vector3& vector);
			friend Vector3 operator*(Vector3& left, Matrix3& right);
			friend Vector3 operator*(Matrix3& left, Vector3& right);
			Vector3& operator*=(Vector3& vector);

			bool operator==(Matrix3& other);

			float Determinant();
			Matrix3 Adjoint();

			void Invert();
			static const Matrix3 Inverse(Matrix3& matrix);

			void Transpose();
			static const Matrix3 Transposition(const Matrix3& matrix);

			static const Matrix3 Identity;
			static const Matrix3 zero;
		};
	}
}