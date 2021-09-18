#include "repch.h"
#include "FrameBuffer.h"

#include "Gl/glew.h"

namespace RealEngine {

	static const uint32_t s_MaxFramebufferSize = 8192;

	static bool IsDepthFormat(FrameBufferTextureFormat format)
	{
		switch (format)
		{
		case FrameBufferTextureFormat::DEPTH24STENCIL8: return true;
		}
		return false;
	}

	FrameBuffer::FrameBuffer(FrameBufferSpecification& spec)
		: m_FrameBufferSpecification(spec)
	{
		for (auto spec : m_FrameBufferSpecification.Attachments.Attachments)
		{
			if (!IsDepthFormat(spec.TextureFormat))
			{
				m_ColorAttachmentSpecs.emplace_back(spec);
			}
			else
			{
				m_DepthAttachmentSpec = spec;
			}
		}

		Create();
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	static GLenum RLTextureFormatToGL(FrameBufferTextureFormat format)
	{
		switch (format)
		{
		case FrameBufferTextureFormat::RGBA8: return GL_RGBA8;
		case FrameBufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
		}
	}

	static GLenum TextureTarget(bool isMultiSample)
	{
		return isMultiSample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static void CreateTextures(bool isMultiSample, uint32_t* outID, uint32_t count)
	{
		glCreateTextures(TextureTarget(isMultiSample), count, outID);
	}

	static void BindTexture(bool isMultiSample, uint32_t id)
	{
		glBindTexture(TextureTarget(isMultiSample), id);
	}

	static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
	{
		bool isMultiSample = samples > 1;
		if (isMultiSample && format != GL_RED_INTEGER)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_TRUE);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(isMultiSample), id, 0);
	}

	static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
	{
		bool isMultiSample = samples > 1;
		if (isMultiSample)
		{
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, format, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentType, GL_RENDERBUFFER, id);
		}
		else
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, TextureTarget(isMultiSample), id, 0);
		}

	}


	void FrameBuffer::Create()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool multisample = m_FrameBufferSpecification.Samples > 1;

		//Attachments
		if (m_ColorAttachmentSpecs.size())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecs.size());

			CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());

			for (size_t i = 0; i < m_ColorAttachments.size(); i++)
			{
				BindTexture(multisample, m_ColorAttachments[i]);

				switch (m_ColorAttachmentSpecs[i].TextureFormat)
				{
				case FrameBufferTextureFormat::RGBA8:
					AttachColorTexture(m_ColorAttachments[i], m_FrameBufferSpecification.Samples, GL_RGBA8, GL_RGBA, m_FrameBufferSpecification.Width, m_FrameBufferSpecification.Height, i);
					break;
				case FrameBufferTextureFormat::RED_INTEGER:
					AttachColorTexture(m_ColorAttachments[i], m_FrameBufferSpecification.Samples, GL_R32I, GL_RED_INTEGER, m_FrameBufferSpecification.Width, m_FrameBufferSpecification.Height, i);
					break;
				}
			}
		}

		if (m_DepthAttachmentSpec.TextureFormat != FrameBufferTextureFormat::None)
		{
			if (!multisample)
			{
				CreateTextures(multisample, &m_DepthAttachment, 1);
				BindTexture(multisample, m_DepthAttachment);
			}
			else
			{
				glGenRenderbuffers(1, &m_DepthAttachment);
				glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachment);
			}

			switch (m_DepthAttachmentSpec.TextureFormat)
			{
			case FrameBufferTextureFormat::DEPTH24STENCIL8:
				AttachDepthTexture(m_DepthAttachment, m_FrameBufferSpecification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_FrameBufferSpecification.Width, m_FrameBufferSpecification.Height);
				break;
			}
		}
	
		if (m_ColorAttachments.size() > 1)
		{
			ENGINE_ASSERT(m_ColorAttachments.size() <= 4, "");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty())
		{
			glDrawBuffer(GL_NONE);
		}

		ENGINE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

	void FrameBuffer::Resize(const glm::vec2& size)
	{
		if (size.x == 0 || size.y == 0 || size.x > s_MaxFramebufferSize || size.y > s_MaxFramebufferSize)
		{
			ENGINE_WARNING("Attempted to resize framebuffer to {0}, {1}", size.x, size.y);
			return;
		}

		m_FrameBufferSpecification.Width = size.x;
		m_FrameBufferSpecification.Height = size.y;

		Create();
	}

	int FrameBuffer::ReadPixels(uint32_t attachmentIndex, int x, int y)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);;

		return pixelData;
	}

	void FrameBuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		auto& spec = m_ColorAttachmentSpecs[attachmentIndex];
		spec.TextureFormat;

		glClearTexImage(m_ColorAttachments[attachmentIndex], 0, RLTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
	}

	void FrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_FrameBufferSpecification.Width, m_FrameBufferSpecification.Height);
	}

	void FrameBuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}