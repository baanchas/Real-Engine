#pragma once

namespace RealEngine {

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer();
		~VertexBuffer();

		void Create();

		void Bind();
		void UnBind();

		void SetData(const void* data, uint32_t size);

	private:
		unsigned int m_RendererID;
	};

}

