#include "repch.h"
#include "ContentBrowserPanel.h"



namespace RealEngine {

	const char* g_AssetsDirectory = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDir(g_AssetsDirectory)
	{
		//m_DirectoryIcon.LoadFromFileFormatted("res/Icons/browser/folder.png");
		//m_FileIcon.LoadFromFileFormatted("res/Icons/browser/file.png");
	}

	ContentBrowserPanel::~ContentBrowserPanel()
	{
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentDir != std::filesystem::path(g_AssetsDirectory))
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentDir = m_CurrentDir.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDir))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, g_AssetsDirectory);
			std::string filenameString = relativePath.filename().string();

			ImGui::PushID(filenameString.c_str());
			Texture2D* icon = directoryEntry.is_directory() ? &m_DirectoryIcon : &m_FileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			/*if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}*/

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					m_CurrentDir /= path.filename();

			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		// TODO: status bar
		ImGui::End();
	}

}
