#include "AABB.h"

namespace Ultrality
{
	namespace Math
	{
		AABB::AABB(const Vector3& min, const Vector3& max)
		{
			this->min = min;
			this->max = max;
		}

		bool AABB::IsPointInsideBox(const Vector3& point)
		{
			DirectX::XMFLOAT3 pointFloat;
			DirectX::XMStoreFloat3(&pointFloat, point.m_Vector);

			DirectX::XMFLOAT3 min;
			DirectX::XMStoreFloat3(&min, this->min.m_Vector);

			DirectX::XMFLOAT3 max;
			DirectX::XMStoreFloat3(&max, this->max.m_Vector);

			if (
				pointFloat.x >= min.x && pointFloat.x <= max.x
				&& pointFloat.y >= min.y && pointFloat.y <= max.y
				&& pointFloat.z >= min.z && pointFloat.z <= max.z
				)
			{
				return true;
			}

			else
			{
				return false;
			}
		}
	}
}