#pragma once

namespace RealEngine {

	class VertexBuffer
	{
	public:
		VertexBuffer() {};
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void Bind();
		void UnBind();

	private:
		unsigned int m_RendererID;
	};

	struct Vec2
	{
		float x, y;
	};

	struct Vec4
	{
		float x, y, z, w;
	};

	struct Vertex
	{
		Vec2 Position;
		Vec4 Color;
	};
}

