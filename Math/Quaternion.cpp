#include "Quaternion.h"
#include <math.h>

namespace Ultrality
{
	namespace Math
	{
		Quaternion::Quaternion()
		{
			this->m_Vector = DirectX::XMVectorZero();
		}
		Quaternion::Quaternion(const enum Axis& axis, const float& angle, const enum Type& angleType)
		{
			switch (angleType)
			{
			case Radians:
				switch (axis)
				{
				case X:
					this->m_Vector = DirectX::XMVectorSet((sinf((angle / 2.0f))), 0.0f, 0.0f, (cosf((angle / 2.0f))));

					break;
				case Y:
					this->m_Vector = DirectX::XMVectorSet(0.0f, (sinf((angle / 2.0f))), 0.0f, (cosf((angle / 2.0f))));

					break;
				case Z:
					this->m_Vector = DirectX::XMVectorSet(0.0f, 0.0f, (sinf((angle / 2.0f))), (cosf((angle / 2.0f))));

					break;
				}
				break;
			case Degrees:
				float radianAngle = DegreesToRadians(angle);
				switch (axis)
				{
				case X:
					this->m_Vector = DirectX::XMVectorSet((sinf((radianAngle / 2.0f))), 0.0f, 0.0f, (cosf((radianAngle / 2.0f))));

					break;
				case Y:
					this->m_Vector = DirectX::XMVectorSet(0.0f, (sinf((radianAngle / 2.0f))), 0.0f, (cosf((radianAngle / 2.0f))));

					break;
				case Z:
					this->m_Vector = DirectX::XMVectorSet(0.0f, 0.0f, (sinf((radianAngle / 2.0f))), (cosf((radianAngle / 2.0f))));

					break;
				}
				break;
			}
		}
		Quaternion::Quaternion(const float& x, const float& y, const float& z, const float& s)
		{
			this->m_Vector = DirectX::XMVectorSet(x, y, z, s);
		}
		Quaternion::Quaternion(DirectX::FXMVECTOR& vector)
		{
			this->m_Vector = vector;
		}
		Quaternion::Quaternion(DirectX::XMFLOAT4& vector)
		{
			this->m_Vector = DirectX::XMLoadFloat4(&vector);
		}
		Quaternion::Quaternion(Vector3& vector)
		{
			this->m_Vector = DirectX::XMVectorSet(vector.X(), vector.Y(), vector.Z(), 0.0f);
		}
		/*Quaternion::Quaternion(Matrix3& matrix)
		{
			float trace = matrix.getIndex(0, 0) + matrix.getIndex(1, 1) + matrix.getIndex(2, 2);

			if (trace > 0.0f)
			{
				float s = sqrtf(trace + 1.0f);
				this->s = s * 0.5f;

				float t = 0.5f / this->s;
				this->x = ((matrix.getIndex(2,1) - matrix.getIndex(1,2)) * t);
				this->y = ((matrix.getIndex(0,2) - matrix.getIndex(2,0)) * t);
				this->z = ((matrix.getIndex(1,0) - matrix.getIndex(0,1)) * t);
			}

			else
			{
				int i = 0;
				if (matrix.getIndex(1, 1) > matrix.getIndex(0, 0)) i = 1;
				if (matrix.getIndex(2, 2) > matrix.getIndex(i, i)) i = 2;

				static const int next[3] = { 1,2,0 };
				int j = next[i];
				int k = next[j];

				float s = sqrtf(matrix.getIndex(i, j) - (matrix.getIndex(j, i) + matrix.getIndex(k, k)) + 1.0f);

				switch (i)
				{
				case 0:
					this->x = s * 0.5f;
				case 1:
					this->y = s * 0.5f;
				case 2:
					this->z = s * 0.5f;
				}
				

				float t;
				if (s != 0.0f) t = 0.5f / s;
				else t = s;

				this->s = ((matrix.getIndex(k, j) - matrix.getIndex(j, k)) * t);

				switch (j)
				{
				case 0:
					this->x = ((matrix.getIndex(j, i) + matrix.getIndex(i, j)) * t);
				case 1:
					this->y = ((matrix.getIndex(j, i) + matrix.getIndex(i, j)) * t);
				case 2:
					this->z = ((matrix.getIndex(j, i) + matrix.getIndex(i, j)) * t);
				}

				switch (k)
				{
				case 0:
					this->x = ((matrix.getIndex(k, i) + matrix.getIndex(i, k)) * 0.5f);
				case 1:
					this->y = ((matrix.getIndex(k, i) + matrix.getIndex(i, k)) * 0.5f);
				case 2:
					this->z = ((matrix.getIndex(k, i) + matrix.getIndex(i, k)) * 0.5f);
				}
			}
		}*/

		float Quaternion::getX()
		{
			DirectX::XMFLOAT4 vector;
			DirectX::XMStoreFloat4(&vector, this->m_Vector);

			return vector.x;
		}
		float Quaternion::getY()
		{
			DirectX::XMFLOAT4 vector;
			DirectX::XMStoreFloat4(&vector, this->m_Vector);

			return vector.y;
		}
		float Quaternion::getZ()
		{
			DirectX::XMFLOAT4 vector;
			DirectX::XMStoreFloat4(&vector, this->m_Vector);

			return vector.z;
		}
		float Quaternion::getS()
		{
			DirectX::XMFLOAT4 vector;
			DirectX::XMStoreFloat4(&vector, this->m_Vector);

			return vector.w;
		}

		Quaternion& Quaternion::MultiplyScalar(const float& scalar)
		{
			this->m_Vector = DirectX::operator*(scalar, this->m_Vector);

			return *this;
		}
		Quaternion operator*(Quaternion left, const float& scalar)
		{
			return left.MultiplyScalar(scalar);
		}
		Quaternion& Quaternion::operator*=(const float& scalar)
		{
			return MultiplyScalar(scalar);
		}

		Quaternion& Quaternion::MultiplyQuaternion(const Quaternion& other)
		{
			this->m_Vector = DirectX::XMQuaternionMultiply(this->m_Vector, other.m_Vector);

			return *this;
		}
		Quaternion operator*(Quaternion left, const Quaternion& right)
		{
			return left.MultiplyQuaternion(right);
		}
		Quaternion& Quaternion::operator*=(const Quaternion& other)
		{
			return MultiplyQuaternion(other);
		}

		bool Quaternion::operator==(const Quaternion& other)
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
		/*const bool Quaternion::operator==(const Quaternion& quat1, const Quaternion& quat2)
		{
			DirectX::XMFLOAT4 row1;
			DirectX::XMStoreFloat4(&row1, DirectX::XMVectorNearEqual(quat1.m_Vector, quat2.m_Vector, Vector4::epsilon.m_Vector));

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

		void Quaternion::Invert()
		{
			this->m_Vector = DirectX::XMQuaternionInverse(this->m_Vector);
		}
		Quaternion Quaternion::Inverse()
		{
			return Quaternion(DirectX::XMQuaternionInverse(this->m_Vector));
		}
		const Quaternion Quaternion::Inverse(const Quaternion& quat)
		{
			return Quaternion(DirectX::XMQuaternionInverse(quat.m_Vector));
		}

		float Quaternion::Magnitude()
		{
			DirectX::XMFLOAT4 vector;
			DirectX::XMStoreFloat4(&vector, DirectX::XMQuaternionLength(this->m_Vector));

			return vector.x;
		}
		float Quaternion::SqrdMagnitude()
		{
			DirectX::XMFLOAT4 vector;
			DirectX::XMStoreFloat4(&vector, DirectX::XMQuaternionLengthSq(this->m_Vector));

			return vector.x;
		}

		Quaternion Quaternion::Normal()
		{
			return Quaternion(DirectX::XMQuaternionNormalize(this->m_Vector));
		}
		void Quaternion::Normalize()
		{
			this->m_Vector = DirectX::XMQuaternionNormalize(this->m_Vector);
		}

		void Quaternion::LERP(const Quaternion& right, const float beta)
		{
			this->m_Vector = DirectX::XMVectorLerp(this->m_Vector, right.m_Vector, beta);
		}
		const Quaternion Quaternion::LERP(const Quaternion& left, const Quaternion& right, const float beta)
		{
			return Quaternion (DirectX::XMVectorLerp(left.m_Vector, right.m_Vector, beta));
		}

		void Quaternion::SLERP(const Quaternion& right, const float beta)
		{
			this->m_Vector = DirectX::XMQuaternionSlerp(this->m_Vector, right.m_Vector, beta);
		}
		const Quaternion Quaternion::SLERP(const Quaternion& left, const Quaternion& right, const float beta)
		{
			return Quaternion(DirectX::XMQuaternionSlerp(left.m_Vector, right.m_Vector, beta));
		}
	}
}