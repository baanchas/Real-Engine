#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace RealEngine {

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void Create();

		void Bind() const;
		void UnBind() const;

		void Addbuffer(VertexBuffer& vb, const VertexBufferLayout& layout);
		void AddVertexBuffer(VertexBuffer& vb);
	private:
		unsigned int m_RendererID;
	};

}
