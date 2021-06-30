#pragma once

#include "entt.hpp"
#include "Components.h"
#include "Renderer/Renderer.h"
#include "Renderer/EditorCamera.h"
#include "SceneSerializer.h"
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
	private:
		template<typename T>
		bool OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registry;

		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};

}