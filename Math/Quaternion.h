#pragma once
#include "CartesianSystems.h"
#include "Vector3.h"
#include "Matrix3.h"
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

namespace Ultrality
{
	namespace Math
	{
		class Vector3;
		class Matrix3;

		class Quaternion
		{
			friend class Matrix4;
			friend class Vector3;
			friend class Vector4;
			//friend class Matrix3;

		private:
			DirectX::XMVECTOR m_Vector;

		public:
			Quaternion();
			Quaternion(const enum Axis& axis, const float& angle, const enum Type& angleType);
			Quaternion(const float& x, const float& y, const float& z, const float& s);
			Quaternion(DirectX::FXMVECTOR& vector);
			Quaternion(DirectX::XMFLOAT4& vector);
			Quaternion(Vector3& vector);
			//Quaternion(Matrix3& matrix);

			float getX();
			float getY();
			float getZ();
			float getS();

			Quaternion& MultiplyScalar(const float& scalar);
			friend Quaternion operator*(Quaternion left, const float& scalar);
			Quaternion& operator*=(const float& scalar);

			Quaternion& MultiplyQuaternion(const Quaternion& other);
			friend Quaternion operator*(Quaternion left, const Quaternion& right);
			Quaternion& operator*=(const Quaternion& other);

			bool operator==(const Quaternion& other);
			//static const bool operator==(const Quaternion& quat1, const Quaternion& quat2);

			void Invert();
			Quaternion Inverse();
			static const Quaternion Inverse(const Quaternion& quat);

			float Magnitude();
			float SqrdMagnitude();

			Quaternion Normal();
			void Normalize();

			void LERP(const Quaternion& right, const float beta);
			static const Quaternion LERP(const Quaternion& left, const Quaternion& right, const float beta);

			void SLERP(const Quaternion& right, const float beta);
			static const Quaternion SLERP(const Quaternion& left, const Quaternion& right, const float beta);
		};
	}
}