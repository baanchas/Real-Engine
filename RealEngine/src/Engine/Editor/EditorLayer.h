#pragma once

#include "Application.h"

#include "Engine/Layers/Layer.h"
#include "Engine/Editor/SceneHierarchyPanel.h"
#include "OpenGL/Shader.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/VertexBuffer.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/VertexBufferLayout.h"
#include "OpenGL/Texture2D.h"

#include "Renderer/Renderer.h"
#include "Scene/Scene.h"
#include "Renderer/FrameBuffer.h"

namespace RealEngine {

	class CameraController;
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer();

		void OnUpdate(float ts) override;
		void OnEvent(Event& event) override;
		void OnImGuiRender() override;
		void OnRender() override;

	private:
		void NewScene();
		void OpenScene();
		void SaveScene();

	private:
		Texture2D SpriteCheckerBoard;

		FrameBuffer* m_FrameBuffer;

		Scene* m_ActiveScene;

		SceneHierarchyPanel m_SceneHierarchyPanel = SceneHierarchyPanel(m_ActiveScene);

		SceneSerializer m_Serializer = SceneSerializer(m_ActiveScene);

		glm::vec2 m_ViewPortSize;

		bool m_SceneWindowIsFocused = false;
	};

}

