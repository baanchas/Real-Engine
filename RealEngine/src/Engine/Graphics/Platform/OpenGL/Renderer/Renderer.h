#pragma once

#include "Engine/Graphics/Platform/OpenGL/VertexArray.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/Shader.h"
#include "OrthographicCamera.h"

namespace RealEngine {

	class Renderer
	{
	public:
		Renderer() {};
		~Renderer() {};

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Clear() { glClear(GL_COLOR_BUFFER_BIT); };
		static void Draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader, const glm::mat4& transform = glm::mat4(1.0f));

	private:
		glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);

		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};

}