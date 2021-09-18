#pragma once

namespace RealEngine {

	

	enum class FrameBufferTextureFormat
	{
		None = 0,

		RGBA8,

		RED_INTEGER,

		DEPTH24STENCIL8,

		Depth = DEPTH24STENCIL8
	};
	
	struct FrameBufferTextureSpecification
	{
		FrameBufferTextureSpecification() = default;
		FrameBufferTextureSpecification(FrameBufferTextureFormat format)
			: TextureFormat(format) {}


		FrameBufferTextureFormat TextureFormat = FrameBufferTextureFormat::None;
	};

	struct FrameBufferAttachmentSpecification
	{
		FrameBufferAttachmentSpecification() = default;
		FrameBufferAttachmentSpecification(const std::initializer_list<FrameBufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<FrameBufferTextureSpecification> Attachments;
	};

	struct FrameBufferSpecification
	{
		uint32_t Width, Height;
		FrameBufferAttachmentSpecification Attachments;
		uint32_t Samples = 1;

		bool m_SwapChainTarget = false;
	};

	class FrameBuffer
	{
	public:
		FrameBuffer(FrameBufferSpecification& spec);
		virtual ~FrameBuffer();

		void Create();
		void Resize(const glm::vec2& size);
		int ReadPixels(uint32_t attachmentIndex, int x, int y);

		void ClearAttachment(uint32_t attachmentIndex, int value);

		void Bind();
		void UnBind();

		inline unsigned int GetColorAttachmentID(uint32_t index = 0) const { ENGINE_ASSERT(index < m_ColorAttachments.size(), "Greater than {0}", m_ColorAttachments.size()); return m_ColorAttachments[index]; }

		inline FrameBufferSpecification GetSpecification() const { return m_FrameBufferSpecification; }


	private:
		FrameBufferSpecification m_FrameBufferSpecification;

		uint32_t m_RendererID = 0;
		
		std::vector<FrameBufferTextureSpecification> m_ColorAttachmentSpecs;
		FrameBufferTextureSpecification m_DepthAttachmentSpec = FrameBufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment;
	};

}
