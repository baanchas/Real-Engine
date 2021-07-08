#pragma once

#include "Scene.h"

namespace RealEngine {

	//class SceneHierarchyPanel;

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity id, Scene* scene);
		~Entity();

		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			ENGINE_ASSERT(!HasComponent<T>(), "Entity already has such component!");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityId, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template <typename T>
		T& GetComponent()
		{
			ENGINE_ASSERT(HasComponent<T>(), "Entity does not have such component!");
			return m_Scene->m_Registry.get<T>(m_EntityId);
		}

		template <typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_EntityId);
		}

		template<typename T>
		void RemoveComponent()
		{
			if (!HasComponent<T>())
			{
				ENGINE_ERROR("Entity does not have such component!");
			}
			T& component = m_Scene->m_Registry.get<T>(m_EntityId);
			m_Scene->OnComponentDeleted<T>(*this, component);
			m_Scene->m_Registry.remove<T>(m_EntityId);
		}

		operator bool() const { return m_EntityId != entt::null; }

		operator uint32_t() const { return (uint32_t)m_EntityId; };
		operator entt::entity() const { return m_EntityId; };

		entt::entity Get() { return m_EntityId; };

		bool operator==(const Entity& other) const
		{
			return m_EntityId == other.m_EntityId && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

	private:
		entt::entity m_EntityId;
		Scene* m_Scene;
	};

	class ScriptableEntity
	{
	public:
		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}
	private:
		Entity m_Entity;
		friend class Scene;
	};

}