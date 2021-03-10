#pragma once

namespace RealEngine {

	struct FrameBufferSpecification
	{
		uint32_t m_Width, m_Height;
		uint32_t m_Samples = 1;

		bool m_SwapChainTarget = false;
	};

	class FrameBuffer
	{
	public:
		FrameBuffer();
		virtual ~FrameBuffer();

		void Create(FrameBufferSpecification& spec);

		void Bind();
		void Unbind();

		inline unsigned int GetColorAttachmentID() const { return m_ColorAttachment; }

		inline FrameBufferSpecification GetSpecification() const { return m_FrameBufferSpecification; }
	private:
		FrameBufferSpecification m_FrameBufferSpecification;

		uint32_t m_RendererID;
		uint32_t m_ColorAttachment, m_DepthAttachment;

	};

}
