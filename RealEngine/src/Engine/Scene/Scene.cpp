#include "repch.h"
#include "Scene.h"

namespace RealEngine {

	//class Entity;

	Scene::Scene()
	{
		
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdate(float ts)
	{
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				if (!nsc.Instance)
				{
					nsc.InstantiateFunction();
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.OnCreateFunction(nsc.Instance);
				}

				nsc.OnUpdateFunction(nsc.Instance, ts);
			});
		}
	}

	void Scene::OnEvent(Event event)
	{
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				if (!nsc.Instance)
				{
					nsc.InstantiateFunction();
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.OnCreateFunction(nsc.Instance);
				}

				nsc.OnEventFunction(nsc.Instance, event);
			});
		}
	}

	void Scene::OnRender()
	{
		// Render
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();

			for (auto entity : view)
			{
				auto [transform, camera] = m_Registry.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer::BeginScene((*mainCamera), cameraTransform);

			auto TCView = m_Registry.view<SpriteRendererComponent>();
			auto TRCView = m_Registry.view<TextureRendererComponent>();

			for (auto entity : TCView)
			{
				if (m_Registry.has<SpriteRendererComponent>(entity))
				{
					
					auto transform = m_Registry.get<TransformComponent>(entity);
					auto sprite = m_Registry.get<SpriteRendererComponent>(entity);

					Renderer::DrawQuad(transform.GetTransform(), sprite);
				}
			}

			for (auto entity : TRCView)
			{
				if (m_Registry.has<TextureRendererComponent>(entity))
				{
					auto transform = m_Registry.get<TransformComponent>(entity);
					auto sprite = m_Registry.get<TextureRendererComponent>(entity).Texture;

					Renderer::DrawQuad(transform.GetTransform(), sprite, 1.0f);
				}
			}

			Renderer::EndScene();
		}

	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = m_Registry.get<CameraComponent>(entity);

			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

	Entity Scene::CreateEntity()
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TagComponent>();
		entity.AddComponent<TransformComponent>();

		return entity;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		auto& tc = entity.AddComponent<TagComponent>();
		tc.Tag = name;
		entity.AddComponent<TransformComponent>();

		return entity;
	}

	template<typename T>
	bool Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	bool Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
		return true;
	}
}