#pragma once

namespace RealEngine {

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexId;
		float TilingFactor;
		int entityID;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer();
		~VertexBuffer();

		void Create(uint32_t size);

		void Bind();
		void UnBind();

		void SetData(const void* data, uint32_t size);

	private:
		unsigned int m_RendererID;
	};

}

