#include "repch.h"
#include "VertexArray.h"


namespace RealEngine {

	VertexArray::VertexArray()
	{
		//glGenVertexArrays(1, &m_RendererID);
		//glBindVertexArray(m_RendererID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void VertexArray::Create()
	{
		glGenVertexArrays(1, &m_RendererID);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void VertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::Addbuffer(VertexBuffer& vb, const VertexBufferLayout& layout)
	{
		glBindVertexArray(m_RendererID);
		//vb.Bind();
		const auto& elements = layout.GetElements();
		unsigned int offset = 0;

		for (unsigned int i = 0; i < elements.size(); i++)
		{
			const auto& element = elements[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
			offset += element.count * VertexBufferElement::GetSize(element.type);
		}
	}

	void VertexArray::AddVertexBuffer(VertexBuffer& vb)
	{
		Bind();
		vb.Bind();

		glEnableVertexArrayAttrib(m_RendererID, 0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));

		glEnableVertexArrayAttrib(m_RendererID, 1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));

		//glCreateBuffers(1, &vb);
	}
}