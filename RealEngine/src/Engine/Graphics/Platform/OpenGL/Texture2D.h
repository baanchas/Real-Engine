#pragma once

#include "stb_image/stb_image.h"


namespace RealEngine {

	class Texture2D
	{
	public:
		Texture2D();
		~Texture2D();

		void Bind(unsigned int slot = 0);
		void UnBind();

		void LoadFromFile(const std::string& path);

		void SetRendererID(unsigned int id) { m_RendererID = id; };

		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }

		inline unsigned int GetRendererID() const { return m_RendererID; }

		bool operator==(Texture2D& other)
		{
			return this->GetRendererID() == other.GetRendererID();
		}
	private:
		unsigned int m_RendererID = 0;
		std::string m_FilePath;
		unsigned char* m_LocalBuffer;
		int m_Width, m_Height, m_BitsPerPixel;
	};

}
