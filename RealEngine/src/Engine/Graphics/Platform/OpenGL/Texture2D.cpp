#include "repch.h"

#include "Engine/Graphics/Platform/OpenGL/Renderer/Renderer.h"
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
	}

	void Texture2D::UnBind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture2D::LoadFromFile(const std::string& path)
	{
		//m_RendererID = 0;
		m_FilePath = path; 
		//

		int width, height, channels;

		stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		m_Width = width;
		m_Height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, GL_RGBA8, m_Width, m_Height);


		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);
		
		//glBindTexture(GL_TEXTURE_2D, m_RendererID);

		//glBindTexture(GL_TEXTURE_2D, 0);

		if (data)
			stbi_image_free(data);
	}

}