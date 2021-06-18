#pragma once

#include "Engine/Graphics/Platform/OpenGL/Scene/Entity.h"
#include "Engine/Graphics/Platform/OpenGL/Scene/Scene.h"

namespace RealEngine {


	class SceneHierarchyPanel 
	{
	public:
		SceneHierarchyPanel(Scene* scene);

		void SetContext(Scene* scene);

		void OnImGuiRender();

		void DrawComponents(Entity entity);
	private:
		Entity m_SelectedItem{ entt::null, m_Context };
		Scene* m_Context = nullptr;
	};

}