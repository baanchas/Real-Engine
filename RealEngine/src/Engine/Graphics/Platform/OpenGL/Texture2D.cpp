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
		m_RendererID = 0;
		m_FilePath = path; 
		m_BitsPerPixel = 0;

		stbi_set_flip_vertically_on_load(1);
		m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BitsPerPixel, 4);

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
		
		glBindTexture(GL_TEXTURE_2D, 0);

		if (m_LocalBuffer)
			stbi_image_free(m_LocalBuffer);
	}

}