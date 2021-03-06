#pragma once

#include "Application.h"

#include "Layer.h"
#include "OpenGL/Shader.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/VertexBuffer.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/VertexBufferLayout.h"
#include "OpenGL/Renderer/Renderer.h"

#include "OpenGL/Renderer/CameraController.h"
#include "OpenGL/Texture2D.h"

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
		Quad quad;
		Texture2D texture;

		float m_CaneraSpeed = 3.0f;
		float m_CameraRotationSpeed = 1.0f;

		float a = 0;


		float m_PosX;
		float m_PosY;
		float m_PosX2;
		float m_PosY2;
		float m_PosX3;
		float m_PosY3;
		
		float w = 0.18f;
		float incr = 0.1f;
	};

}

