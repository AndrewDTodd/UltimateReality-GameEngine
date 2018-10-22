#include "Vector3.h"
#include <math.h>
#include <iostream>

namespace Ultrality
{
	namespace Math
	{
		Vector3::Vector3()
		{
			this->m_Vector = DirectX::XMVectorZero();
		}
		Vector3::Vector3(const float& x, const float& y, const float& z)
		{
			this->m_Vector = DirectX::XMVectorSet(x, y, z, 1.0f);
		}
		Vector3::Vector3(Vector4& vec4)
		{
			this->m_Vector = vec4.m_Vector;
		}
		Vector3::Vector3(DirectX::FXMVECTOR vector)
		{
			this->m_Vector = vector;
		}

		float Vector3::X()
		{
			DirectX::XMFLOAT3 vector;
			DirectX::XMStoreFloat3(&vector, this->m_Vector);

			return vector.x;
		}
		float Vector3::Y()
		{
			DirectX::XMFLOAT3 vector;
			DirectX::XMStoreFloat3(&vector, this->m_Vector);

			return vector.y;
		}
		float Vector3::Z()
		{
			DirectX::XMFLOAT3 vector;
			DirectX::XMStoreFloat3(&vector, this->m_Vector);

			return vector.z;
		}

		void Vector3::setX(const float& x)
		{
			this->m_Vector = DirectX::XMVectorSetX(this->m_Vector, x);
		}
		void Vector3::setY(const float& y)
		{
			this->m_Vector = DirectX::XMVectorSetY(this->m_Vector, y);
		}
		void Vector3::setZ(const float& z)
		{
			this->m_Vector = DirectX::XMVectorSetZ(this->m_Vector, z);
		}

		std::ostream& operator<<(std::ostream& stream, Vector3& vec3)
		{
			DirectX::XMFLOAT3 vector;
			DirectX::XMStoreFloat3(&vector, vec3.m_Vector);

			stream << "[" << vector.x << "," << vector.y << "," << vector.z << "]";

			return stream;
		}

		Vector3& Vector3::MultiplyScalar(const float& scalar)
		{
			this->m_Vector = DirectX::operator*(scalar, this->m_Vector);

			return *this;
		}
		Vector3 operator*(Vector3 left, const float& scalar)
		{
			return left.MultiplyScalar(scalar);
		}
		Vector3 operator*(const float& scalar, Vector3 right)
		{
			return right.MultiplyScalar(scalar);
		}
		Vector3& Vector3::operator*=(const float& scalar)
		{
			return MultiplyScalar(scalar);
		}

		Vector3& Vector3::MultiplyVector(const Vector3& other)
		{
			this->m_Vector = DirectX::operator*(other.m_Vector, this->m_Vector);

			return *this;
		}
		Vector3 operator*(Vector3 left, const Vector3& right)
		{
			return left.MultiplyVector(right);
		}
		Vector3& Vector3::operator*=(const Vector3& other)
		{
			return MultiplyVector(other);
		}

		Vector3& Vector3::Add(const Vector3& other)
		{
			this->m_Vector = DirectX::operator+(other.m_Vector);

			return *this;
		}
		Vector3 operator+(Vector3 left, const Vector3& right)
		{
			return left.Add(right);
		}
		Vector3& Vector3::operator+=(const Vector3& other)
		{
			return Add(other);
		}

		bool Vector3::operator==(const Vector3& other)
		{
			DirectX::XMFLOAT3 row1;
			DirectX::XMStoreFloat3(&row1, DirectX::XMVectorNearEqual(this->m_Vector, other.m_Vector, Vector4::epsilon.m_Vector));

			if
			(
				row1.x == 1
			)
			{
				return true;
			}

			else
			{
				return false;
			}
		}
		/*const bool Vector3::operator==(const Vector3& vec1, const Vector3& vec2)
		{
			DirectX::XMFLOAT3 row1;
			DirectX::XMStoreFloat3(&row1, DirectX::XMVectorNearEqual(vec1.m_Vector, vec2.m_Vector, Vector4::epsilon.m_Vector));

			if
			(
				row1.x == 1
			)
			{
				return true;
			}

			else
			{
				return false;
			}
		}*/

		Vector3& Vector3::Subtract(const Vector3& other)
		{
			this->m_Vector = DirectX::operator-(other.m_Vector);

			return *this;
		}
		Vector3 operator-(Vector3 left, const Vector3& right)
		{
			return left.Subtract(right);
		}
		Vector3& Vector3::operator-=(const Vector3& other)
		{
			return Subtract(other);
		}

		float Vector3::Magnitude()
		{
			DirectX::XMVECTOR vector = DirectX::operator*(this->m_Vector, this->m_Vector);

			float sum = vector.m128_f32[0];
			sum += vector.m128_f32[1];
			sum += vector.m128_f32[2];

			return sqrtf(sum);
		}
		float Vector3::SqrdMagnitude()
		{
			DirectX::XMVECTOR vector = DirectX::operator*(this->m_Vector, this->m_Vector);

			float sum = vector.m128_f32[0];
			sum += vector.m128_f32[1];
			sum += vector.m128_f32[2];

			return sum;
		}

		const float Vector3::DistanceBetweenPoints(const Vector3& pointOne, const Vector3& pointTwo)
		{
			DirectX::XMVECTOR vector = DirectX::operator-(pointTwo.m_Vector, pointOne.m_Vector);

			vector = DirectX::operator*(vector, vector);

			float sum = vector.m128_f32[0];
			sum += vector.m128_f32[1];
			sum += vector.m128_f32[2];

			return sqrtf(sum);
		}
		const float Vector3::SqrdDistanceBetweenPoints(const Vector3& pointOne, const Vector3& pointTwo)
		{

			DirectX::XMVECTOR vector = DirectX::operator-(pointTwo.m_Vector, pointOne.m_Vector);

			vector = DirectX::operator*(vector, vector);

			float sum = vector.m128_f32[0];
			sum += vector.m128_f32[1];
			sum += vector.m128_f32[2];

			return sum;
		}

		Vector3 Vector3::Normal()
		{
			Vector3 normal(DirectX::XMVector3Normalize(this->m_Vector));

			return normal;
		}
		void Vector3::Normalize()
		{
			this->m_Vector = DirectX::XMVector3Normalize(this->m_Vector);
		}

		const float Vector3::DotProduct(const Vector3& left, const Vector3& right)
		{
			DirectX::XMFLOAT3 Dot;
			DirectX::XMStoreFloat3(&Dot, DirectX::XMVector3Length(DirectX::XMVector3Dot(left.m_Vector, right.m_Vector)));

			return Dot.x;
		}
		float Vector3::DotProduct(const Vector3& other)
		{
			DirectX::XMFLOAT3 Dot;
			DirectX::XMStoreFloat3(&Dot, DirectX::XMVector3Dot(this->m_Vector, other.m_Vector));

			return Dot.x;
		}

		bool Vector3::Collinear(Vector3& a, Vector3& b)
		{
			if (DotProduct(a, b) == (a.Magnitude() * b.Magnitude()))
			{
				return true;
			}

			else
			{
				return false;
			}
		}
		bool Vector3::OppositeCollinear(Vector3& a, Vector3& b)
		{
			if (DotProduct(a, b) == (-(a.Magnitude() * b.Magnitude())))
			{
				return true;
			}

			else
			{
				return false;
			}
		}
		bool Vector3::Perpendicular(Vector3& a, Vector3& b)
		{
			if (DotProduct(a, b) == 0.0f)
			{
				return true;
			}

			else
			{
				return false;
			}
		}
		bool Vector3::SameDirection(Vector3& a, Vector3& b)
		{
			if (DotProduct(a, b) > 0.0f)
			{
				return true;
			}

			else
			{
				return false;
			}
		}
		bool Vector3::OppositeDirection(Vector3& a, Vector3& b)
		{
			if (DotProduct(a, b) < 0.0f)
			{
				return true;
			}

			else
			{
				return false;
			}
		}
		

		const Vector3& Vector3::CrossProduct(const Vector3& left, const Vector3& right)
		{
			static Vector3 cross; 
			cross.m_Vector = DirectX::XMVector3Cross(left.m_Vector, right.m_Vector);

			return cross;
		}

		void Vector3::LERP(const Vector3& right, const float& beta)
		{
			this->m_Vector = DirectX::XMVectorLerp(this->m_Vector, right.m_Vector, beta);
		}
		const Vector3 Vector3::LERP(Vector3& left, const Vector3& right, const float& beta)
		{
			return Vector3(DirectX::XMVectorLerp(left.m_Vector, right.m_Vector, beta));
		}

		void Vector3::RotateVectorByQuaternion(const Quaternion& quat)
		{
			this->m_Vector = DirectX::XMVector3Rotate(this->m_Vector, quat.m_Vector);
		}
		const Vector3 Vector3::RotateVectorByQuaternion(const Vector3& vec, const Quaternion& quat)
		{
			return Vector3(DirectX::XMVector3Rotate(vec.m_Vector, quat.m_Vector));
		}

		const Vector3 Vector3::zero(0.0f, 0.0f, 0.0f);
		const Vector3 Vector3::one(1.0f, 1.0f, 1.0f);
		const Vector3 Vector3::up(0.0f, 1.0f, 0.0f);
		const Vector3 Vector3::forward(0.0f, 0.0f, 1.0f);
		const Vector3 Vector3::right(1.0f, 0.0f, 0.0f);
		const Vector3 Vector3::null(0.0f, 0.0f, 0.0f);
	}
}