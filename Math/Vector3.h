#pragma once
#include <iostream>
#include "Vector4.h"
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include "Quaternion.h"

namespace Ultrality
{
	namespace Math
	{
		class Vector4;

		class Vector3
		{
			friend class Vector4;
			friend class AABB;
			friend class OBB;
			friend class Matrix3;
			friend class Matrix4;
			friend class Quaternion;

		private:
			DirectX::XMVECTOR m_Vector;

		public:
			Vector3();
			Vector3(const float& x, const float& y, const float& z);
			Vector3(Vector4& vec4);
			Vector3(DirectX::FXMVECTOR vector);

			float X();
			float Y();
			float Z();

			void setX(const float& x);
			void setY(const float& y);
			void setZ(const float& z);

			friend std::ostream& operator<<(std::ostream& stream, Vector3& vec3);

			Vector3& MultiplyScalar(const float& scalar);
			friend Vector3 operator*(Vector3 left, const float& scalar);
			friend Vector3 operator*(const float& scalar, Vector3 right);
			Vector3& operator*=(const float& scalar);

			Vector3& MultiplyVector(const Vector3& other);
			friend Vector3 operator*(Vector3 left, const Vector3& right);
			Vector3& operator*=(const Vector3& other);

			Vector3& Add(const Vector3& other);
			friend Vector3 operator+(Vector3 left, const Vector3& right);
			Vector3& operator+=(const Vector3& other);

			Vector3& Subtract(const Vector3& other);
			friend Vector3 operator-(Vector3 left, const Vector3& rigth);
			Vector3& operator-=(const Vector3& other);

			bool operator==(const Vector3& other);
			//static const bool operator==(const Vector3& vec1, const Vector3& vec2);

			float Magnitude();
			float SqrdMagnitude();

			static const float DistanceBetweenPoints(const Vector3& pointOne, const Vector3& pointTwo);
			static const float SqrdDistanceBetweenPoints(const Vector3& pointOne, const Vector3& pointTwo);

			Vector3 Normal();
			void Normalize();

			static const float DotProduct(const Vector3& left, const Vector3& right);
			float DotProduct(const Vector3& other);
			
			bool Collinear(Vector3& a, Vector3& b);
			bool OppositeCollinear(Vector3& a, Vector3& b);
			bool Perpendicular(Vector3& a, Vector3& b);
			bool SameDirection(Vector3& a, Vector3& b);
			bool OppositeDirection(Vector3& a, Vector3& b);
			

			static const Vector3& CrossProduct(const Vector3& left, const Vector3& right);

			void LERP(const Vector3& right, const float& beta);
			static const Vector3 LERP(Vector3& left, const Vector3& right, const float& beta);

			void RotateVectorByQuaternion(const Quaternion& quat);
			static const Vector3 RotateVectorByQuaternion(const Vector3& vec, const Quaternion& quat);

			static const Vector3 zero;
			static const Vector3 one;
			static const Vector3 up;
			static const Vector3 forward;
			static const Vector3 right;
			static const Vector3 null;
		};
	}
}