#pragma once

#include "Application.h"

#include "Layer.h"
#include "OpenGL/Shader.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/VertexBuffer.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/VertexBufferLayout.h"
#include "OpenGL/Renderer/Renderer.h"
#include "OpenGL/Scene/Scene.h"

#include "OpenGL/Renderer/CameraController.h"
#include "OpenGL/Texture2D.h"
#include "OpenGL/Renderer/FrameBuffer.h"

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
		CameraController m_CameraController;

		Texture2D texture;

		Quad quad;

		FrameBuffer* m_FrameBuffer;

		Scene* m_ActiveScene;

		Entity square;

		glm::vec2 m_ViewPortSize;

		bool m_SceneWindowIsFocused = false;

		float m_PosX = 100.0f;
		float m_PosY = 100.0f;

	};

}

