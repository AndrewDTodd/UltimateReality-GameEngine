#include "Vector4.h"
#include <math.h>
#include <iostream>

namespace Ultrality
{
	namespace Math
	{
		Vector4::Vector4()
		{
			this->m_Vector = DirectX::XMVectorZero();
		}
		Vector4::Vector4(const float& x, const float& y, const float& z, const float& w)
		{
			this->m_Vector = DirectX::XMVectorSet(x, y, z, w);
		}
		Vector4::Vector4(Vector3& vec3)
		{
			DirectX::XMFLOAT4 vect;
			DirectX::XMStoreFloat4(&vect, vec3.m_Vector);

			vect.w = 1.0f;
			this->m_Vector = DirectX::XMLoadFloat4(&vect);
		}
		Vector4::Vector4(Quaternion& quaternion)
		{
			this->m_Vector = quaternion.m_Vector;
		}
		Vector4::Vector4(DirectX::FXMVECTOR vec)
		{
			this->m_Vector = vec;
		}

		float Vector4::X()
		{
			DirectX::XMFLOAT4 vect;
			DirectX::XMStoreFloat4(&vect, this->m_Vector);

			return vect.x;
		}
		float Vector4::Y()
		{
			DirectX::XMFLOAT4 vect;
			DirectX::XMStoreFloat4(&vect, this->m_Vector);

			return vect.y;
		}
		float Vector4::Z()
		{
			DirectX::XMFLOAT4 vect;
			DirectX::XMStoreFloat4(&vect, this->m_Vector);

			return vect.z;
		}
		float Vector4::W()
		{
			DirectX::XMFLOAT4 vect;
			DirectX::XMStoreFloat4(&vect, this->m_Vector);

			return vect.w;
		}

		void Vector4::setX(const float& x)
		{
			this->m_Vector = DirectX::XMVectorSetX(this->m_Vector, x);
		}
		void Vector4::setY(const float& y)
		{
			this->m_Vector = DirectX::XMVectorSetY(this->m_Vector, y);
		}
		void Vector4::setZ(const float& z)
		{
			this->m_Vector = DirectX::XMVectorSetZ(this->m_Vector, z);
		}
		void Vector4::setW(const float& w)
		{
			this->m_Vector = DirectX::XMVectorSetW(this->m_Vector, w);
		}


		std::ostream& operator<<(std::ostream& stream, Vector4& vec4)
		{
			DirectX::XMFLOAT4 vector;
			DirectX::XMStoreFloat4(&vector, vec4.m_Vector);

			stream << "[" << vector.x << "," << vector.y << "," << vector.z << "," << vector.w << "]";

			return stream;
		}

		Vector4& Vector4::MultiplyScalar(const float& scalar)
		{
			this->m_Vector = DirectX::operator*(scalar, this->m_Vector);

			return *this;
		}
		Vector4 operator*(Vector4 left, const float& scalar)
		{
			return left.MultiplyScalar(scalar);
		}
		Vector4& Vector4::operator*=(const float& scalar)
		{
			return MultiplyScalar(scalar);
		}

		Vector4& Vector4::MultiplyVector(const Vector4& other)
		{
			this->m_Vector = DirectX::operator*(other.m_Vector, this->m_Vector);

			return *this;
		}
		Vector4 operator*(Vector4 left, const Vector4& right)
		{
			return left.MultiplyVector(right);
		}
		Vector4& Vector4::operator*=(const Vector4& other)
		{
			return MultiplyVector(other);
		}

		Vector4& Vector4::Add(const Vector4& other)
		{
			this->m_Vector = DirectX::operator+(other.m_Vector);

			return *this;
		}
		Vector4 operator+(Vector4 left, const Vector4& right)
		{
			return left.Add(right);
		}
		Vector4& Vector4::operator+=(const Vector4& other)
		{
			return Add(other);
		}

		bool Vector4::operator==(const Vector4& other)
		{
			DirectX::XMFLOAT4 row1;
			DirectX::XMStoreFloat4(&row1, DirectX::XMVectorNearEqual(this->m_Vector, other.m_Vector, Vector4::epsilon.m_Vector));

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
		/*const bool Vector4::operator==(const Vector4& vec1, const Vector4& vec2)
		{
			DirectX::XMFLOAT4 row1;
			DirectX::XMStoreFloat4(&row1, DirectX::XMVectorNearEqual(vec1.m_Vector, vec2.m_Vector, Vector4::epsilon.m_Vector));

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

		float Vector4::Magnitude()
		{
			DirectX::XMFLOAT4 vector;
			DirectX::XMStoreFloat4(&vector, DirectX::XMVector4Length(this->m_Vector));

			return vector.x;
		}
		float Vector4::SqrdMagnitude()
		{
			DirectX::XMFLOAT4 vector;
			DirectX::XMStoreFloat4(&vector, DirectX::XMVector4LengthSq(this->m_Vector));

			return vector.x;
		}

		Vector4 Vector4::Normal()
		{
			Vector4 normal(DirectX::XMVector4Normalize(this->m_Vector));

			return normal;
		}
		void Vector4::Normalize()
		{
			this->m_Vector = DirectX::XMVector4Normalize(this->m_Vector);
		}

		float Vector4::DotProduct(const Vector4& left, const Vector4& right)
		{
			DirectX::XMFLOAT4 Dot;
			DirectX::XMStoreFloat4(&Dot, DirectX::XMVector3Length(DirectX::XMVector4Dot(left.m_Vector, right.m_Vector)));

			return Dot.x;
		}
		float Vector4::DotProduct(const Vector4& other)
		{
			DirectX::XMFLOAT4 Dot;
			DirectX::XMStoreFloat4(&Dot, DirectX::XMVector4Dot(this->m_Vector, other.m_Vector));

			return Dot.x;
		}

		bool Vector4::Collinear(Vector4& a, Vector4& b)
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
		bool Vector4::OppositeCollinear(Vector4& a, Vector4& b)
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
		bool Vector4::Perpendicular(Vector4& a, Vector4& b)
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
		bool Vector4::SameDirection(Vector4& a, Vector4& b)
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
		bool Vector4::OppositeDirection(Vector4& a, Vector4& b)
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

		const Vector4& Vector4::CrossProduct(const Vector4& V1, const Vector4& V2, const Vector4& V3)
		{
			static Vector4 cross;
			cross.m_Vector = DirectX::XMVector4Cross(V1.m_Vector, V2.m_Vector, V3.m_Vector);

			return cross;
		}

		void Vector4::LERP(const Vector4& right, const float beta)
		{
			this->m_Vector = DirectX::XMVectorLerp(this->m_Vector, right.m_Vector, beta);
		}
		const Vector4 Vector4::LERP(Vector4& left, const Vector4& right, const float beta)
		{
			return Vector3(DirectX::XMVectorLerp(left.m_Vector, right.m_Vector, beta));
		}

		const Vector4 Vector4::zero(0.0f, 0.0f, 0.0f, 0.0f);
		const Vector4 Vector4::up(0.0f, 1.0f, 0.0f, 1.0f);
		const Vector4 Vector4::forward(0.0f, 0.0f, 1.0f, 1.0f);
		const Vector4 Vector4::right(1.0f, 0.0f, 0.0f, 1.0f);
		const Vector4 Vector4::epsilon(0.0001f, 0.0001f, 0.0001f, 0.0001f);
	}
}