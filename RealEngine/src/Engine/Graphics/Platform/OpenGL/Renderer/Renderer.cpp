#include "repch.h"
#include "Renderer.h"

namespace RealEngine {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	
	}

	void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, Shader& shader, const glm::mat4& transform)
	{
		shader.Bind();
		shader.SetUniformMat4f("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		shader.SetUniformMat4f("u_Transform", transform);

		vertexArray.Bind();
		indexBuffer.Bind();
		glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
	}

}
