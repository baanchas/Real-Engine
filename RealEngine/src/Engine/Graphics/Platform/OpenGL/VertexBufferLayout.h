#pragma once


namespace RealEngine {

	enum class VertexBufferElementType
	{
		FLOAT = 0,
		INT,
		UNSIGNED_INT,
		UNSIGNED_BYTE,
		MAT4
	};

	static GLenum VertexBufferElementTypeToOpenGLBaseType(VertexBufferElementType type)
	{
		switch (type)
		{
		case VertexBufferElementType::FLOAT:			return GL_FLOAT;
		case VertexBufferElementType::INT:				return GL_INT;
		case VertexBufferElementType::UNSIGNED_INT:		return GL_UNSIGNED_INT;
		case VertexBufferElementType::UNSIGNED_BYTE:	return GL_UNSIGNED_BYTE;
		case VertexBufferElementType::MAT4:				return GL_FLOAT;
		}

		return 0;
	}

	struct VertexBufferElement
	{
		VertexBufferElementType type;
		unsigned int count;
		unsigned char normalized;

		static unsigned int GetSize(VertexBufferElementType type)
		{
			switch (type)
			{
			case VertexBufferElementType::FLOAT:			return 4;
			case VertexBufferElementType::INT:				return 4;
			case VertexBufferElementType::UNSIGNED_INT:		return 4;
			case VertexBufferElementType::UNSIGNED_BYTE:	return 1;
			case VertexBufferElementType::MAT4:				return 4 * 4 * 4;
			}
			return 0;
		}
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout() {};
		~VertexBufferLayout() {};

		template<typename T>
		void Push(unsigned int count)
		{
			static_assert(false);
		}

		template<>
		void Push<float>(unsigned int count)
		{
			m_Elements.push_back({ VertexBufferElementType::FLOAT, count, GL_FALSE });
			m_Stride += VertexBufferElement::GetSize(VertexBufferElementType::FLOAT) * count;
		}

		template<>
		void Push<unsigned int>(unsigned int count)
		{
			m_Elements.push_back({ VertexBufferElementType::UNSIGNED_INT, count, GL_FALSE });
			m_Stride += VertexBufferElement::GetSize(VertexBufferElementType::UNSIGNED_INT) * count;
		}

		template<>
		void Push<int>(unsigned int count)
		{
			m_Elements.push_back({ VertexBufferElementType::INT, count, GL_FALSE });
			m_Stride += VertexBufferElement::GetSize( VertexBufferElementType::INT) * count;
		}

		template<>
		void Push<unsigned char>(unsigned int count)
		{
			m_Elements.push_back({ VertexBufferElementType::UNSIGNED_BYTE, count, GL_TRUE });
			m_Stride += VertexBufferElement::GetSize(VertexBufferElementType::UNSIGNED_BYTE) * count;
		}
		template<>
		void Push<glm::mat4>(unsigned int count)
		{
			m_Elements.push_back({ VertexBufferElementType::MAT4, count, GL_FALSE });
			m_Stride += VertexBufferElement::GetSize(VertexBufferElementType::MAT4) * count;
		}

		inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; };
		inline unsigned int GetStride() const { return m_Stride; };

	private:
		std::vector<VertexBufferElement> m_Elements;
		unsigned int m_Stride = 0;
	};


}