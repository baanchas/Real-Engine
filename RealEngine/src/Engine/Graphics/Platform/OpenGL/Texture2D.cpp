#include "repch.h"

#include "Renderer/Renderer.h"
#include "Texture2D.h"

namespace RealEngine {

	Texture2D::Texture2D()
	{
	
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void Texture2D::Bind(unsigned int slot)
	{
		glBindTextureUnit(slot, m_RendererID);
		//glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void Texture2D::UnBind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture2D::LoadFromFile(const std::string& path)
	{
		if (!path.empty())
		{
			m_FilePath = path;

			int width, height, channels;

			stbi_set_flip_vertically_on_load(1);

			stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
			ENGINE_ASSERT(data, "OpenGLTexture2D:: Failed to load Image!");


			m_Width = width;
			m_Height = height;

			GLenum internalFormat = 0, format = 0;

			if (channels == 1)
			{
				internalFormat = GL_R32I;
				format = GL_RED;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				format = GL_RGB;
			}
			else if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				format = GL_RGBA;
			}

			ENGINE_ASSERT(internalFormat && format, "OpenGLTexture2D:: Format not supported!");

			glGenTextures(1, &m_RendererID);
			glBindTexture(GL_TEXTURE_2D, m_RendererID);

			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


			if (data)
				stbi_image_free(data);
		}
		else
		{
			ENGINE_ERROR("The path string is EMPTY!");
		}

	}

	void Texture2D::LoadFromFileFormatted(const std::string& path)
	{
		m_FilePath = path;


		stbi_set_flip_vertically_on_load(1);

		int width, height, channels;
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		ENGINE_ASSERT(data, "Failed to load Image");
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, format = 0;

		if (channels == 1)
		{
			format = GL_RED;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			format = GL_RGB;
		}
		else if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			format = GL_RGBA;
		}

		ENGINE_ASSERT(internalFormat && format, "OpenGLTexture2D:: Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, format, GL_UNSIGNED_BYTE, data);

		if (data)
			stbi_image_free(data);
	}

}