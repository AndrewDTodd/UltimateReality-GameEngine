#pragma once
#include <iostream>
#include "Vector3.h"
#include "Quaternion.h"
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

namespace Ultrality
{
	namespace Math
	{
		class Vector3;
		class Quaternion;

		class Vector4
		{
			friend class Quaternion;
			friend class Vector3;
			friend class Matrix4;

		private:
			DirectX::XMVECTOR m_Vector;

		public:
			Vector4();
			Vector4(const float& x, const float& y, const float& z, const float& w);
			Vector4(Vector3& vec3);
			Vector4(Quaternion& quaternion);
			Vector4(DirectX::FXMVECTOR vec);

			float X();
			float Y();
			float Z();
			float W();

			void setX(const float& x);
			void setY(const float& y);
			void setZ(const float& z);
			void setW(const float& w);

			friend std::ostream& operator<<(std::ostream& stream, Vector4& vec4);

			Vector4& MultiplyScalar(const float& scalar);
			friend Vector4 operator*(Vector4 left, const float& scalar);
			Vector4& operator*=(const float& scalar);

			Vector4& MultiplyVector(const Vector4& other);
			friend Vector4 operator*(Vector4 left, const Vector4& right);
			Vector4& operator*=(const Vector4& other);

			Vector4& Add(const Vector4& other);
			friend Vector4 operator+(Vector4 left, const Vector4& right);
			Vector4& operator+=(const Vector4& other);

			bool operator==(const Vector4& other);
			//static const bool operator==(const Vector4& vec1, const Vector4& vec2);

			float Magnitude();
			float SqrdMagnitude();

			Vector4 Normal();
			void Normalize();

			static float DotProduct(const Vector4& left, const Vector4& right);
			float DotProduct(const Vector4& other);

			bool Collinear(Vector4& a, Vector4& b);
			bool OppositeCollinear(Vector4& a, Vector4& b);
			bool Perpendicular(Vector4& a, Vector4& b);
			bool SameDirection(Vector4& a, Vector4& b);
			bool OppositeDirection(Vector4& a, Vector4& b);

			static const Vector4& CrossProduct(const Vector4& V1, const Vector4& V2, const Vector4& V3);

			void LERP(const Vector4& right, const float beta);
			static const Vector4 LERP(Vector4& left, const Vector4& right, const float beta);

			static const Vector4 zero;
			static const Vector4 up;
			static const Vector4 forward;
			static const Vector4 right;
			static const Vector4 epsilon;
		};
	}
}