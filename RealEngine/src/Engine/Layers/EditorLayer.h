#pragma once

#include "Layer.h"
#include "Platform/OpenGL/Shader.h"
#include "Platform/OpenGL/VertexArray.h"
#include "Platform/OpenGL/VertexBuffer.h"
#include "Platform/OpenGL/IndexBuffer.h"
#include "Platform/OpenGL/VertexBufferLayout.h"
#include "Platform/OpenGL/Renderer.h"

namespace RealEngine {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer();

		void OnUpdate() override;
		void OnEvent() override;
		void OnRender() override;

	private:
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

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec4 a_Position;
			layout(location = 1) in vec4 a_Color;
            
            uniform mat4 u_MVP;

            out vec4 v_Color;

			void main()
			{
                v_Color = a_Color;
				gl_Position = u_MVP * a_Position;	
            }
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
            
            in vec4 v_Color;
                        
			void main()
			{
            	color = v_Color;
			}
		)";
		Shader m_Shader = Shader(vertexSrc, fragmentSrc);
		VertexBuffer m_VertexBuffer = VertexBuffer(positionsRect, sizeof(positionsRect));
		IndexBuffer m_IndexBuffer = IndexBuffer(indices, 12);

		VertexArray m_VertexArray;
		VertexBufferLayout m_Layout;
		
		float w = 0.18f;
		float incr = 0.1f;
	};

}

