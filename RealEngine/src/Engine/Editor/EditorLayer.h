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
		Texture2D SpriteCheckerBoard;

		FrameBuffer* m_FrameBuffer;

		Scene* m_ActiveScene;

		SceneHierarchyPanel m_SceneHierarchyPanel = SceneHierarchyPanel(m_ActiveScene);

		SceneSerializer serializer = SceneSerializer(m_ActiveScene);
		
		Entity square;
		Entity square2;
		Entity square3;
		Entity m_CameraEntity;
		Entity m_CameraEntity2;


		glm::vec2 m_ViewPortSize;

		bool m_SceneWindowIsFocused = false;

		float m_PosX = 100.0f;
		float m_PosY = 100.0f;

	};

}

