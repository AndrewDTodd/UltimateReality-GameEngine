#pragma once
#include "Vector3.h"
#include "Quaternion.h"

namespace Ultrality
{
	namespace Math
	{
		class UniformSQT
		{
		private:
			float scale;
			Quaternion rotation;
			Vector3 translation;

		public:
			UniformSQT(const float& scale, const Quaternion& rotation, const Vector3& translation);
		};

		class NonUniformSQT
		{
		private:
			Vector3 scale;
			Quaternion rotation;
			Vector3 translation;

		public:
			NonUniformSQT(const Vector3& scale, const Quaternion& rotation, const Vector3& translation);
		};
	}
}