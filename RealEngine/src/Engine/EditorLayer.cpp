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
		ENGINE_INFO("Editor Layer Updatefunc");
	}

	void EditorLayer::OnEvent()
	{
	}

	void EditorLayer::OnRender()
	{
	}
}
