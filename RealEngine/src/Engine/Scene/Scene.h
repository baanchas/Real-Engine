#pragma once

#include "entt.hpp"
#include "Components.h"
#include "Renderer/Renderer.h"
#include "Renderer/EditorCamera.h"
#include "SceneSerializer.h"
#include "Models/ObjectLoader.h"

namespace RealEngine {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity();
		Entity CreateEntity(const std::string& name);

		void OnUpdate(float ts);
		void OnEvent(Event event);
		void OnRenderEditor(EditorCamera& camera);
		void OnRenderRuntime();
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCameraEntity();

		inline std::string& GetTitle() { return m_Title; };
		void SetTitle(const std::string& title) { m_Title = title; };
	private:
		template<typename T>
		bool OnComponentAdded(Entity entity, T& component);

		template<typename T>
		bool OnComponentDeleted(Entity entity, T& component);

	private:
		std::string m_Title = "Untitled";

		std::vector<glm::vec3> vertices;
		std::vector<glm::uint32_t> indices;

		entt::registry m_Registry;

		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		uint32_t* temp;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};

}