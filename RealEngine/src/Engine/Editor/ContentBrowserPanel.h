#pragma once 

#include <filesystem>

namespace RealEngine {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		~ContentBrowserPanel();

		void OnImGuiRender(); 
		
		Texture2D m_DirectoryIcon;
		Texture2D m_FileIcon;

	private:

		std::filesystem::path m_CurrentDir;
		
	};

}