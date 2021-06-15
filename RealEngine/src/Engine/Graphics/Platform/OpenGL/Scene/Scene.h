#pragma once

#include "entt.hpp"
#include "Components.h"
#include "Engine/Graphics/Platform/OpenGL/Renderer/Renderer.h"
#include "Entity.h"

namespace RealEngine {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity();

		void OnUpdate();
		void OnViewportResize(uint32_t width, uint32_t height);
	private:
		entt::registry m_Registry;

		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		friend class Entity;
	};

}