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

namespace RealEngine {

	class CameraController;
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer();

		void OnUpdate(float ts) override;
		void OnEvent(Event& event) override;
		void OnRender() override;

	private:
		CameraController m_CameraController;
		
		float m_CaneraSpeed = 3.0f;
		float m_CameraRotationSpeed = 1.0f;

		float a = 0;


		float m_PosX;
		float m_PosY;
		float m_PosX2;
		float m_PosY2;

		glm::vec3 translationA = glm::vec3(0, 0, 0);
		glm::vec3 translationB = glm::vec3(0, 0, 0);
		glm::vec3 translationC = glm::vec3(0, 0, 0);
		glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f); // projection
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // "Camera" pos
		glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA); // "Models" pos
		//glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB); // "Models" pos
		glm::mat4 mvp = projection * view * model;

		Vertex m_Verticies[8];

		
		Quad quad;


		
		float w = 0.18f;
		float incr = 0.1f;
	};

}

