#include "repch.h"
#include "EditorLayer.h"

namespace RealEngine {

	EditorLayer::EditorLayer()
	{
		ENGINE_INFO("Editor Layer is pushed");
	}

	EditorLayer::~EditorLayer()
	{
	}

	void EditorLayer::OnUpdate()
	{
		ENGINE_INFO("{0}", a);
	}

	void EditorLayer::OnEvent()
	{
	}

	void EditorLayer::OnRender()
	{
		ImGui::ShowDemoWindow();

		ImGui::Begin("Test");
		ImGui::DragFloat("dragfloat", &a, 1.0f);
		ImGui::End();

	}
}
