#pragma once

#include <array>

namespace RealEngine {

	struct Vertex
	{
		glm::vec2 Position;
		glm::vec4 Color;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer() {};
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void Bind();
		void UnBind();

		static std::array<Vertex, 4> CreateQuad(float x, float y, float size);

	private:
		unsigned int m_RendererID;
	};

}

