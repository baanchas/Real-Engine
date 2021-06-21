#pragma once

#include "entt.hpp"
#include "Components.h"
#include "Engine/Graphics/Platform/OpenGL/Renderer/Renderer.h"

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
		void OnRender();
		void OnViewportResize(uint32_t width, uint32_t height);
	private:
		entt::registry m_Registry;

		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		int p = 5;

		friend class Entity;
		friend class SceneHierarchyPanel;
	};

}