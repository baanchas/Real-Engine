#include "repch.h"
#include "Scene.h"

namespace RealEngine {

	Scene::Scene()
	{
		m_LightTexture.LoadFromFileFormatted("res/sprites/components/light.png");
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
		Renderer::EndSkyBoxScene();

		Renderer::BeginScene(camera);

		glm::vec3* lightPositions = m_SceneLightsPositionsBase;
		glm::vec3* lightColors = m_SceneLightsColorsBase;

		for (unsigned int i = 0; i < m_SceneLightsPositions.size(); ++i)
		{
			lightPositions[i] = m_SceneLightsPositions[i];
			lightColors[i] = m_SceneLightsColors[i];
		}

		Renderer::UploadUniformInt("u_LightCount", m_SceneLightsPositions.size());
		Renderer::UploadUniformVec3Array("u_LightPositions", lightPositions, m_SceneLightsPositions.size());
		Renderer::UploadUniformVec3Array("u_LightColors", lightColors, m_SceneLightsPositions.size());

		glm::vec3* spotlightPositions = m_SceneSpotLightsPositionsBase;
		glm::vec3* spotlightDirections = m_SceneSpotLightsDirectionssBase;

		for (unsigned int i = 0; i < m_SceneSpotLightsPositions.size(); ++i)
		{
			spotlightPositions[i] = m_SceneSpotLightsPositions[i];
			spotlightDirections[i] = m_SceneSpotLightsDirections[i];
		}

		Renderer::UploadUniformInt("u_SpotLightCount", m_SceneSpotLightsPositions.size());
		Renderer::UploadUniformVec3Array("u_SpotLightPositions", spotlightPositions, m_SceneSpotLightsPositions.size());
		Renderer::UploadUniformVec3Array("u_SpotLightColors", spotlightDirections, m_SceneSpotLightsDirections.size());
		Renderer::UploadUniformFloatArray("u_SpotLightCutOff", m_SceneSpotLightsCutOff.data(), m_SceneSpotLightsCutOff.size());


		m_SceneLightsPositions.clear();
		m_SceneLightsColors.clear();

		m_SceneSpotLightsPositions.clear();
		m_SceneSpotLightsDirections.clear();
		m_SceneSpotLightsCutOff.clear();

		Renderer::UploadUniformVec3Float("u_ViewPosition", glm::vec3{camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z});
		Renderer::UploadUniformFloat("u_AmbientOcclusion", 1.0f);


		auto TCView = m_Registry.view<SpriteRenderer>();
		auto TRCView = m_Registry.view<TextureRenderer>();
		//auto TMeshCView = m_Registry.view<TexturedMeshComponent>();
		auto MeshCView = m_Registry.view<MeshComponent>();
		auto LightCView = m_Registry.view<PointLight>();
		auto SpotLightCView = m_Registry.view<Spotlight>();

		/*for (auto entity : MeshCView)
		{
			if (m_Registry.has<MeshComponent>(entity))
			{
				auto& mesh = m_Registry.get<MeshComponent>(entity).ownMesh;
				auto& transform = m_Registry.get<TransformComponent>(entity);

				Renderer::DrawMesh(transform.GetTransform(), mesh, (int)entity);
			}
		}*/

		for (auto entity : MeshCView)
		{
			if (m_Registry.has<MeshComponent>(entity))
			{
				auto& mesh = m_Registry.get<MeshComponent>(entity);
				auto& textures = m_Registry.get<MeshComponent>(entity).Textures;
				auto& transform = m_Registry.get<TransformComponent>(entity);

				Renderer::DrawMesh(transform.GetTransform(), mesh.ownMesh, textures, mesh.isTexturedProperty, (int)entity);
			}
		}

		for (auto entity : TCView)
		{
			if (m_Registry.has<SpriteRenderer>(entity))
			{

				auto& transform = m_Registry.get<TransformComponent>(entity);
				auto& sprite = m_Registry.get<SpriteRenderer>(entity);

				Renderer::DrawQuad(transform.GetTransform(), sprite, int(entity));
			}
		}

		for (auto entity : TRCView)
		{
			if (m_Registry.has<TextureRenderer>(entity))
			{
				auto& transform = m_Registry.get<TransformComponent>(entity);
				auto& sprite = m_Registry.get<TextureRenderer>(entity);

				if (sprite.Texture != nullptr)
					Renderer::DrawQuad(transform.GetTransform(), sprite.Texture, 1.0f, (int)entity);
			}
		}


		for (auto entity : LightCView)
		{
			if (m_Registry.has<PointLight>(entity))
			{
				auto& transform = m_Registry.get<TransformComponent>(entity);
				auto& light = m_Registry.get<PointLight>(entity);

				Renderer::DrawLight(transform.GetTransform(), &m_LightTexture, (int)entity);
				m_SceneLightsPositions.push_back(transform.Position);
				m_SceneLightsColors.push_back({ light.Color.r * light.ColorStrength, light.Color.g * light.ColorStrength, light.Color.b * light.ColorStrength, });
			}
		}

		for (auto entity : SpotLightCView)
		{
			if (m_Registry.has<Spotlight>(entity))
			{
				auto& transform = m_Registry.get<TransformComponent>(entity);
				auto& light = m_Registry.get<Spotlight>(entity);

				Renderer::DrawLight(transform.GetTransform(), &m_LightTexture, (int)entity);
				m_SceneSpotLightsPositions.push_back(transform.Position);
				m_SceneSpotLightsDirections.push_back({ light.Direction.r, light.Direction.g, light.Direction.b, });
				m_SceneSpotLightsCutOff.push_back(light.CutOff);
			}
		}

		Renderer::BindTextures();

		Renderer::StartBatch();
	}


	void Scene::OnRenderRuntime()
	{
		// Render
		Camera* mainCamera = nullptr;
		glm::vec3 cameraPos = { 0.0f, 0.0f, 0.0f };
		glm::mat4 cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();

			for (auto entity : view)
			{
				auto [transform, camera] = m_Registry.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					mainCamera->Move(transform.Position);
					/*mainCamera->OnUpdate(2);
					mainCamera->Rotate(transform.Rotation);*/
					cameraPos = transform.Position;
					//transform.Rotation = glm::vec3( transform.Rotation.x, -transform.Rotation.y, transform.Rotation.z );
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer::BeginSkyBoxScene(*mainCamera, cameraTransform);
			Renderer::EndSkyBoxScene();

			Renderer::BeginScene((*mainCamera), cameraTransform);

			glm::vec3* lightPositions = m_SceneLightsPositionsBase;
			glm::vec3* lightColors = m_SceneLightsColorsBase;

			for (unsigned int i = 0; i < m_SceneLightsPositions.size(); ++i)
			{
				lightPositions[i] = m_SceneLightsPositions[i];
				lightColors[i] = m_SceneLightsColors[i];
			}

			Renderer::UploadUniformInt("u_LightCount", m_SceneLightsPositions.size());
			Renderer::UploadUniformVec3Array("u_LightPositions", lightPositions, m_SceneLightsPositions.size());
			Renderer::UploadUniformVec3Array("u_LightColors", lightColors, m_SceneLightsPositions.size());

			glm::vec3* spotlightPositions = m_SceneSpotLightsPositionsBase;
			glm::vec3* spotlightDirections = m_SceneSpotLightsDirectionssBase;

			for (unsigned int i = 0; i < m_SceneSpotLightsPositions.size(); ++i)
			{
				spotlightPositions[i] = m_SceneSpotLightsPositions[i];
				spotlightDirections[i] = m_SceneSpotLightsDirections[i];
			}

			Renderer::UploadUniformInt("u_SpotLightCount", m_SceneSpotLightsPositions.size());
			Renderer::UploadUniformVec3Array("u_SpotLightPositions", spotlightPositions, m_SceneSpotLightsPositions.size());
			Renderer::UploadUniformVec3Array("u_SpotLightColors", spotlightDirections, m_SceneSpotLightsDirections.size());
			Renderer::UploadUniformFloatArray("u_SpotLightCutOff", m_SceneSpotLightsCutOff.data(), m_SceneSpotLightsCutOff.size());


			m_SceneLightsPositions.clear();
			m_SceneLightsColors.clear();

			m_SceneSpotLightsPositions.clear();
			m_SceneSpotLightsDirections.clear();
			m_SceneSpotLightsCutOff.clear();

			Renderer::UploadUniformVec3Float("u_ViewPosition", cameraPos);
			Renderer::UploadUniformFloat("u_AmbientOcclusion", 1.0f);


			auto TCView = m_Registry.view<SpriteRenderer>();
			auto TRCView = m_Registry.view<TextureRenderer>();
			//auto TMeshCView = m_Registry.view<TexturedMeshComponent>();
			auto MeshCView = m_Registry.view<MeshComponent>();
			auto LightCView = m_Registry.view<PointLight>();
			auto SpotLightCView = m_Registry.view<Spotlight>();

			/*for (auto entity : MeshCView)
			{
				if (m_Registry.has<MeshComponent>(entity))
				{
					auto& mesh = m_Registry.get<MeshComponent>(entity).ownMesh;
					auto& transform = m_Registry.get<TransformComponent>(entity);

					Renderer::DrawMesh(transform.GetTransform(), mesh, (int)entity);
				}
			}*/

			for (auto entity : MeshCView)
			{
				if (m_Registry.has<MeshComponent>(entity))
				{
					auto& mesh = m_Registry.get<MeshComponent>(entity);
					auto& textures = m_Registry.get<MeshComponent>(entity).Textures;
					auto& transform = m_Registry.get<TransformComponent>(entity);

					Renderer::DrawMesh(transform.GetTransform(), mesh.ownMesh, textures, mesh.isTexturedProperty, (int)entity);
				}
			}

			for (auto entity : TCView)
			{
				if (m_Registry.has<SpriteRenderer>(entity))
				{

					auto& transform = m_Registry.get<TransformComponent>(entity);
					auto& sprite = m_Registry.get<SpriteRenderer>(entity);

					Renderer::DrawQuad(transform.GetTransform(), sprite, int(entity));
				}
			}

			for (auto entity : TRCView)
			{
				if (m_Registry.has<TextureRenderer>(entity))
				{
					auto& transform = m_Registry.get<TransformComponent>(entity);
					auto& sprite = m_Registry.get<TextureRenderer>(entity);

					if (sprite.Texture != nullptr)
						Renderer::DrawQuad(transform.GetTransform(), sprite.Texture, 1.0f, (int)entity);
				}
			}


			for (auto entity : LightCView)
			{
				if (m_Registry.has<PointLight>(entity))
				{
					auto& transform = m_Registry.get<TransformComponent>(entity);
					auto& light = m_Registry.get<PointLight>(entity);

					//Texture2D* texture;
					//m_LightTexture.UnBind();
					//Renderer::DrawLight(transform.GetTransform(), &m_LightTexture, (int)entity);
					//m_LightTexture.Bind();
					m_SceneLightsPositions.push_back(transform.Position);
					m_SceneLightsColors.push_back({ light.Color.r * light.ColorStrength, light.Color.g * light.ColorStrength, light.Color.b * light.ColorStrength, });
				}
			}

			for (auto entity : SpotLightCView)
			{
				if (m_Registry.has<Spotlight>(entity))
				{
					auto& transform = m_Registry.get<TransformComponent>(entity);
					auto& light = m_Registry.get<Spotlight>(entity);

					Renderer::DrawLight(transform.GetTransform(), &m_LightTexture, (int)entity);
					m_SceneSpotLightsPositions.push_back(transform.Position);
					m_SceneSpotLightsDirections.push_back({ light.Direction.r, light.Direction.g, light.Direction.b, });
					m_SceneSpotLightsCutOff.push_back(light.CutOff);
				}
			}

			Renderer::BindTextures();

			Renderer::StartBatch();
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
		ENGINE_TRACE("\t[{0}]::Tag Component added to entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
		ENGINE_TRACE("\t[{0}]::Transform Component added to entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<SpriteRenderer>(Entity entity, SpriteRenderer& component)
	{
		ENGINE_TRACE("\t[{0}]::Sprite Component added to entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<TextureRenderer>(Entity entity, TextureRenderer& component)
	{
		ENGINE_TRACE("\t[{0}]::Texture Component added to entity with id {1}", m_Title, entity.Get());
		component.Texture = new Texture2D;
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<PointLight>(Entity entity, PointLight& component)
	{
		ENGINE_TRACE("\t[{0}]::Light Component added to entity with id {1}", m_Title, entity.Get());
		//m_SceneLightsPositions.push_back(component);
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
		ENGINE_TRACE("t\[{0}]::Native Script Component added to entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
		ENGINE_TRACE("\t[{0}]::Camera Component added to entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentAdded<MeshComponent>(Entity entity, MeshComponent& component)
	{
		ENGINE_TRACE("\t[{0}]::Mesh Component added to entity with id {1}", m_Title, entity.Get());

		for (int i = 0; i < 5; i++)
		{
			component.isTexturedProperty[i] = false;
		}
		component.Textures.resize(5);
		component.ownMesh.m_Material.AO = 1.0f;
		for (int i = 0; i < 5; i++)
		{
			component.Textures[i].reset(new Texture2D());
		}
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
	bool Scene::OnComponentDeleted<SpriteRenderer>(Entity entity, SpriteRenderer& component)
	{
		ENGINE_WARNING("[{0}]::Sprite Component has been deleted from entity with id {1}", m_Title, entity.Get());
		return true;
	}

	template<>
	bool Scene::OnComponentDeleted<TextureRenderer>(Entity entity, TextureRenderer& component)
	{
		ENGINE_WARNING("[{0}]::Texture Component has been deleted from entity with id {1}", m_Title, entity.Get());
		//delete component.Texture;
		return true;
	}

	template<>
	bool Scene::OnComponentDeleted<PointLight>(Entity entity, PointLight& component)
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
	bool Scene::OnComponentDeleted<MeshComponent>(Entity entity, MeshComponent& component)
	{
		ENGINE_WARNING("[{0}]::MeshComponent has been deleted from entity with id {1}", m_Title, entity.Get());
		delete component.ownMesh.VerticesBase;
		delete component.ownMesh.IndicesBase;
		return true;
	}
}