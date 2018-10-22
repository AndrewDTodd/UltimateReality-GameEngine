#pragma once

#include "../Math/Matrix4.h"
#include <iostream>
#include <string>

namespace Ultrality
{
	namespace GameObjects
	{
		class GameObject
		{
		private:
			Ultrality::Math::Matrix4 m_transform;

			GameObject* m_parent;
			std::string m_name;

		public:
			GameObject(const Ultrality::Math::Matrix4& transform = Ultrality::Math::Matrix4::zero, GameObject* parent = &World, std::string&& name = "New_Game_Object_");
			~GameObject();

			//Ultrality::Math::Matrix4 ToParentSpace();

			friend std::ostream& operator<<(std::ostream& stream, GameObject& gameObject);

			static GameObject World;
		};
	}
}
