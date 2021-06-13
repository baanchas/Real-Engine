#include "repch.h"
#include "Scene.h"

namespace RealEngine {

	//class Entity;

	static void DoMath(const glm::mat4& transform)
	{
	}

	Scene::Scene()
	{
		
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdate()
	{
		auto TCView = m_Registry.view<SpriteRendererComponent>();

		for (auto entity : TCView)
		{
			if (m_Registry.has<SpriteRendererComponent>(entity))
			{
				auto transform = m_Registry.get<TransformComponent>(entity);
				auto sprite = m_Registry.get<SpriteRendererComponent>(entity);

				Renderer::DrawQuad(transform, sprite);
			}
		}

		/*auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			
			
		}*/
	}

	Entity Scene::CreateEntity()
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();

		return entity;
	}
}