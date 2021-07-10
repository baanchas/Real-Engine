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
			if (elements[i].type == VertexBufferElementType::INT)
			{
				glVertexAttribIPointer(i, element.count, VertexBufferElementTypeToOpenGLBaseType(element.type), layout.GetStride(), (const void*)offset);
				offset += element.count * VertexBufferElement::GetSize(element.type);
			}
			else
			{
				glVertexAttribPointer(i, element.count, VertexBufferElementTypeToOpenGLBaseType(element.type), element.normalized, layout.GetStride(), (const void*)offset);
				offset += element.count * VertexBufferElement::GetSize(element.type);
			}
		}
	}

}