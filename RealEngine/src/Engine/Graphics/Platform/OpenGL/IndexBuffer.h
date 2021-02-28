#pragma once


namespace RealEngine {

	class IndexBuffer
	{
	public:
		IndexBuffer() {};
		IndexBuffer(const unsigned int* data, unsigned int count);
		~IndexBuffer();

		void Create(const unsigned int* data, unsigned int count);

		void Bind() const;
		void UnBind() const;

		inline unsigned int GetCount() const { return m_Count; }

	private:
		unsigned int m_RendererID;
		unsigned int m_Count;

	};

}
