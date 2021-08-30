#include "repch.h"
#include "Scene.h"

namespace RealEngine {

	Scene::Scene()
	{
		m_LightTexture.LoadFromFileFormatted("res/sprites/components/lightimage.png");
		m_SceneLightsPositionsBase = new glm::vec3[500];
		m_SceneLightsColorsBase = new glm::vec3[500];
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
		Renderer::BeginSkyBoxScene(camera);
		Renderer::EndSceneCubeMap();

		Renderer::BeginScene(camera);

		glm::vec3* lightPositions = m_SceneLightsPositionsBase;
		glm::vec3* lightColors = m_SceneLightsColorsBase;

		for (unsigned int i = 0; i < m_SceneLightsPositions.size(); ++i)
		{
			lightPositions[i] = m_SceneLightsPositions[i];
			lightColors[i] = m_SceneLightsColors[i];
		}

		Renderer::SetUniform1i("u_LightCount", m_SceneLightsPositions.size());
		Renderer::SetUniform3fArray("u_LightPositions", lightPositions, m_SceneLightsPositions.size());
		Renderer::SetUniform3fArray("u_LightColors", lightColors, m_SceneLightsPositions.size());

		m_SceneLightsPositions.clear();
		m_SceneLightsColors.clear();

		Renderer::SetUniform3f("u_ViewPos", glm::vec3{camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z});
		Renderer::SetUniform1f("u_AmbientOcclusion", 1.0f);


		auto TCView = m_Registry.view<SpriteRendererComponent>();
		auto TRCView = m_Registry.view<TextureRendererComponent>();
		auto TMeshCView = m_Registry.view<TexturedMeshComponent>();
		auto MeshCView = m_Registry.view<MeshComponent>();
		auto LightCView = m_Registry.view<Light>();

		for (auto entity : MeshCView)
		{
			if (m_Registry.has<MeshComponent>(entity))
			{
				auto& mesh = m_Registry.get<MeshComponent>(entity).ownMesh;
				auto& transform = m_Registry.get<TransformComponent>(entity);

				Renderer::DrawMesh(transform.GetTransform(), mesh, (int)entity);
			}
		}

		for (auto entity : TMeshCView)
		{
			if (m_Registry.has<TexturedMeshComponent>(entity))
			{
				auto& mesh = m_Registry.get<TexturedMeshComponent>(entity).ownMesh;
				auto& textures = m_Registry.get<TexturedMeshComponent>(entity).Textures;
				auto& transform = m_Registry.get<TransformComponent>(entity);

				Renderer::DrawMesh(transform.GetTransform(), mesh, textures, (int)entity);
			}
		}

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
				auto& sprite = m_Registry.get<TextureRendererComponent>(entity);

				if (sprite.Texture != nullptr)
					Renderer::DrawQuad(transform.GetTransform(), sprite.Texture, 1.0f, (int)entity);
			}
		}


		for (auto entity : LightCView)
		{
			if (m_Registry.has<Light>(entity))
			{
				auto& transform = m_Registry.get<TransformComponent>(entity);
				auto& light = m_Registry.get<Light>(entity);

				Renderer::DrawLight(transform.GetTransform(), light, &m_LightTexture, (int)entity);
				m_SceneLightsPositions.push_back(transform.Position);
				m_SceneLightsColors.push_back({ light.Color.r * light.ColorStrength, light.Color.g * light.ColorStrength, light.Color.b * light.ColorStrength, });
			}
		}

		Renderer::BindTextures();
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
		ENGINE_INFO("[{0}]::New Entity has been created with id {1}", m_Title, entity.Get());
		entity.AddComponent<TagComponent>();
		entity.AddComponent<TransformComponent>();
		return entity;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		ENGINE_INFO("[{0}]::New Entity has been created with id {1}", m_Title, entity.Get());
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
		ENGINE_TRACE("[Scene::{0}]::Tag Component added to entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
		ENGINE_TRACE("[{0}]::Transform Component added to entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
		ENGINE_TRACE("[{0}]::Sprite Component added to entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<TextureRendererComponent>(Entity entity, TextureRendererComponent& component)
	{
		ENGINE_TRACE("[{0}]::Texture Component added to entity with id {1}", m_Title, entity.Get());
		component.Texture = new Texture2D;
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<Light>(Entity entity, Light& component)
	{
		ENGINE_TRACE("[{0}]::Light Component added to entity with id {1}", m_Title, entity.Get());
		//m_SceneLightsPositions.push_back(component);
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
		ENGINE_TRACE("[{0}]::Native Script Component added to entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
		ENGINE_TRACE("[{0}]::Camera Component added to entity with id {1}", m_Title, entity.Get());
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
		ENGINE_TRACE("[{0}]::Mesh Component added to entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<TexturedMeshComponent>(Entity entity, TexturedMeshComponent& component)
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
		ENGINE_WARNING("[{0}]::Tag Component has been deleted from entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentDeleted<TransformComponent>(Entity entity, TransformComponent& component)
	{
		ENGINE_WARNING("[{0}]::Transform Component has been deleted from entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentDeleted<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
		ENGINE_WARNING("[{0}]::Sprite Component has been deleted from entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentDeleted<TextureRendererComponent>(Entity entity, TextureRendererComponent& component)
	{
		ENGINE_WARNING("[{0}]::Texture Component has been deleted from entity with id {1}", m_Title, entity.Get());
		//delete component.Texture;
		return true;
	}

	template<>
	bool Scene::OnComponentDeleted<Light>(Entity entity, Light& component)
	{
		ENGINE_WARNING("[{0}]::Texture Component has been deleted from entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentDeleted<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
		ENGINE_WARNING("[{0}]::Native Script Component has been deleted from entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentDeleted<CameraComponent>(Entity entity, CameraComponent& component)
	{
		ENGINE_WARNING("[{0}]::Camera Component has been deleted from entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentDeleted<ModelComponent>(Entity entity, ModelComponent& component)
	{
		ENGINE_WARNING("[{0}]::ModelComponent has been deleted from entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentDeleted<MeshComponent>(Entity entity, MeshComponent& component)
	{
		ENGINE_WARNING("[{0}]::MeshComponent has been deleted from entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentDeleted<TexturedMeshComponent>(Entity entity, TexturedMeshComponent& component)
	{
		ENGINE_WARNING("[{0}]::TexturedMeshComponent has been deleted from entity with id {1}", m_Title, entity.Get());
		return true;
	}
}