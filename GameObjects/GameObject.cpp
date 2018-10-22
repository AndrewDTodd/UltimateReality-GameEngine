#include "GameObject.h"

namespace Ultrality
{
	namespace GameObjects
	{
		GameObject::GameObject(const Ultrality::Math::Matrix4& transform, GameObject* parent, std::string&& name)
		{
			this->m_transform = transform;

			this->m_parent = parent;

			this->m_name = name;
		}

		GameObject::~GameObject()
		{
			if (this->m_parent != nullptr)
			{
				this->m_parent = nullptr;
			}
		}

		std::ostream& operator<<(std::ostream& stream, GameObject& gameObject)
		{
			stream << gameObject.m_name << "\n" << gameObject.m_transform;
			return stream;
		}

		GameObject GameObject::World(Ultrality::Math::Matrix4::zero, nullptr, "World");
	}
}