#include "SQT.h"

namespace Ultrality
{
	namespace Math
	{
		UniformSQT::UniformSQT(const float& scale, const Quaternion& rotation, const Vector3& translation)
		{
			this->scale = scale;
			this->rotation = rotation;
			this->translation = translation;
		}

		NonUniformSQT::NonUniformSQT(const Vector3& scale, const Quaternion& rotation, const Vector3& translation)
		{
			this->scale = scale;
			this->rotation = rotation;
			this->translation = translation;
		}
	}
}