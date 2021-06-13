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
	private:
		entt::registry m_Registry;

		friend class Entity;
	};

}