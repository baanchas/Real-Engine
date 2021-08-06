#include "repch.h"
#include "Scene.h"

namespace RealEngine {

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

	void Scene::OnRenderEditor(EditorCamera& camera)
	{
		Renderer::BeginScene(camera);

		Renderer::SetUniform3f("lightPos", glm::vec3{0.0f, 10.0f, 10.0f});
		Renderer::SetUniform3f("viewPos", glm::vec3{camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z});

		auto TCView = m_Registry.view<SpriteRendererComponent>();
		auto TRCView = m_Registry.view<TextureRendererComponent>();
		auto MCView = m_Registry.view<ModelComponent>();
		auto MeshCView = m_Registry.view<MeshComponent>();

		for (auto entity : TCView)
		{
			if (m_Registry.has<SpriteRendererComponent>(entity))
			{

				auto& transform = m_Registry.get<TransformComponent>(entity);
				auto& sprite = m_Registry.get<SpriteRendererComponent>(entity);

				Renderer::DrawQuad(transform.GetTransform(), sprite, int(entity));
			}
		}

		for (auto entity : TRCView)
		{
			if (m_Registry.has<TextureRendererComponent>(entity))
			{
				auto& transform = m_Registry.get<TransformComponent>(entity);
				auto& sprite = m_Registry.get<TextureRendererComponent>(entity).Texture;

				Renderer::DrawQuad(transform.GetTransform(), sprite, 1.0f, (int)entity);
			}
		}

		for (auto entity : MCView)
		{
			if (m_Registry.has<ModelComponent>(entity))
			{
				auto& model = m_Registry.get<ModelComponent>(entity);
				auto& transform = m_Registry.get<TransformComponent>(entity);

				Renderer::DrawModel(transform.GetTransform(), model.Vertices, model.Indices, (int)entity);
			}
		}

		for (auto entity : MeshCView)
		{
			if (m_Registry.has<MeshComponent>(entity))
			{
				auto& mesh = m_Registry.get<MeshComponent>(entity).ownMesh;
				auto& transform = m_Registry.get<TransformComponent>(entity);

				Renderer::DrawMesh(transform.GetTransform(), mesh, (int)entity);
			}
		}
		glm::mat4 trans(1.0f);

	

		//Renderer::DrawMesh(trans, mesh, mat, 60);
		//Renderer::DrawQuad(1.0f, 1.0f, 1.0f, );
		//Renderer::DrawModel(verticesCone, indicesCone);
		//Renderer::DrawModel(verticesAxe, indicesAxe);

		Renderer::EndScene();
	}


	void Scene::OnRenderRuntime()
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

					Renderer::DrawQuad(transform.GetTransform(), sprite, int(entity));
				}
			}

			for (auto entity : TRCView)
			{
				if (m_Registry.has<TextureRendererComponent>(entity))
				{
					auto transform = m_Registry.get<TransformComponent>(entity);
					auto sprite = m_Registry.get<TextureRendererComponent>(entity).Texture;

					Renderer::DrawQuad(transform.GetTransform(), sprite, 1.0f, (int)entity);
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

	Entity Scene::GetPrimaryCameraEntity()
	{
		Entity entity{ entt::null, this };
		auto cameraEntities = m_Registry.view<CameraComponent>();
		for (auto entityID : cameraEntities)
		{
			auto& cc = m_Registry.get<CameraComponent>(entityID);
			if (cc.Primary)
			{
				entity = Entity{ entityID, this };
			}
		}
		return entity;
	}

	Entity Scene::CreateEntity()
	{
		Entity entity = { m_Registry.create(), this };
		ENGINE_INFO("[{0}]::New Entity has been created with id {0}", m_Title, entity.Get());
		entity.AddComponent<TagComponent>();
		entity.AddComponent<TransformComponent>();
		return entity;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		ENGINE_INFO("[{0}]::New Entity has been created with id {0}", m_Title, entity.Get());
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
		ENGINE_INFO("[Scene::{0}]::Tag Component added to entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
		ENGINE_INFO("[{0}]::Transform Component added to entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
		ENGINE_INFO("[{0}]::Sprite Component added to entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<TextureRendererComponent>(Entity entity, TextureRendererComponent& component)
	{
		ENGINE_INFO("[{0}]::Texture Component added to entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
		ENGINE_INFO("[{0}]::Native Script Component added to entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
		ENGINE_INFO("[{0}]::Camera Component added to entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<ModelComponent>(Entity entity, ModelComponent& component)
	{
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<MeshComponent>(Entity entity, MeshComponent& component)
	{
		return true;
	}


	template<typename T>
	bool Scene::OnComponentDeleted(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	bool Scene::OnComponentDeleted<TagComponent>(Entity entity, TagComponent& component)
	{
		ENGINE_INFO("[{0}]::Tag Component has been deleted from entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentDeleted<TransformComponent>(Entity entity, TransformComponent& component)
	{
		ENGINE_INFO("[{0}]::Transform Component has been deleted from entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentDeleted<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
		ENGINE_INFO("[{0}]::Sprite Component has been deleted from entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentDeleted<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
		ENGINE_INFO("[{0}]::Native Script Component has been deleted from entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentDeleted<CameraComponent>(Entity entity, CameraComponent& component)
	{
		ENGINE_INFO("[{0}]::Camera Component has been deleted from entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentDeleted<ModelComponent>(Entity entity, ModelComponent& component)
	{
		ENGINE_INFO("[{0}]::ModelComponent has been deleted from entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentDeleted<MeshComponent>(Entity entity, MeshComponent& component)
	{
		ENGINE_INFO("[{0}]::MeshComponent has been deleted from entity with id {1}", m_Title, entity.Get());
		return true;
	}
}