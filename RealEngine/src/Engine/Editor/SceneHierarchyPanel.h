#pragma once

#include "Scene/Entity.h"
#include "Scene/Scene.h"

namespace RealEngine {


	class SceneHierarchyPanel 
	{
	public:
		SceneHierarchyPanel(Scene* scene);

		Entity GetSelectedEntity() { return m_SelectedItem; }

		void SetContext(Scene* scene);

		void OnImGuiRender();

		void DrawComponents(Entity entity);
	private:
		Entity m_SelectedItem{ entt::null, m_Context };
		Scene* m_Context = nullptr;
	};

}