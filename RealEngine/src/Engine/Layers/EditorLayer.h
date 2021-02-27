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

#include "OpenGL/Renderer/Renderer.h"

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
		unsigned int indices[12] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

		float m_PosX = 100;
		float m_PosY = 100;
		float m_PosX2 = 100;
		float m_PosY2 = 100;

		float positionsRect[48] = {
			 100.0f, 100.0f, 0.18f, 0.6, 0.96, 1.0f,
			 300.0f, 100.0f, 0.18f, 0.6, 0.96, 1.0f,
			 300.0f, 300.0f, 0.18f, 0.6, 0.96, 1.0f,
			 100.0f, 300.0f, 0.18f, 0.6, 0.96, 1.0f,

			 100.0f,  100.0f, 1.0f, 0.93f, 0.24f, 1.0f,
			 300.0f,  100.0f, 1.0f, 0.93f, 0.24f, 1.0f,
			 300.0f,  300.0f, 1.0f, 0.93f, 0.24f, 1.0f,
			 100.0f,  300.0f, 1.0f, 0.93f, 0.24f, 1.0f
		};

		glm::vec3 translationA = glm::vec3(0, 0, 0);
		glm::vec3 translationB = glm::vec3(0, 0, 0);
		glm::vec3 translationC = glm::vec3(0, 0, 0);
		glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f); // projection
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // "Camera" pos
		glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA); // "Models" pos
		//glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB); // "Models" pos
		glm::mat4 mvp = projection * view * model;

		

		Vertex m_Vertices[8];

		Shader* m_Shader; 
		VertexBuffer* m_VertexBuffer; 
		IndexBuffer* m_IndexBuffer;

		VertexArray m_VertexArray;
		VertexBufferLayout m_Layout;
		
		float w = 0.18f;
		float incr = 0.1f;
	};

}

