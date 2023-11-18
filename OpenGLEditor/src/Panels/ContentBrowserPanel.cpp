#include "ContentBrowserPanel.h"

#include <OpenGLEngine/Renderer/Renderer.h>

#include "imgui.h"

namespace OpenGLEngine
{
	ContentBrowserPanel::ContentBrowserPanel() : m_BaseDirectory("Assets"), m_CurrentDirectory(m_BaseDirectory)
	{
		m_DirectoryIcon = Texture::CreateTexture("Icons/texture_dossier.png");
		m_FilePNGIcon = Texture::CreateTexture("Icons/texture_png.png");
		m_FileJPGIcon = Texture::CreateTexture("Icons/texture_jpg.png");
		m_FileOBJIcon = Texture::CreateTexture("Icons/texture_obj.png");
		m_FileOtherIcon = Texture::CreateTexture("Icons/texture_texte.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != std::filesystem::path(m_BaseDirectory))
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Refresh"))
		{
			
		}

		static float padding = 16.0f;
		static float thumbnailSize = 64.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			std::string filenameString = path.filename().string();
			std::filesystem::path relativePath(path);
			std::string itemPath = relativePath.string();

			ImGui::PushID(filenameString.c_str());

			std::shared_ptr<Texture> icon;
			if (directoryEntry.is_directory())
			{
				icon = m_DirectoryIcon;
			}
			else if (GetFileExtension(directoryEntry) == "obj")
			{
				icon = m_FileOBJIcon;
			}
			else if (GetFileExtension(directoryEntry) == "png")
			{
				//icon = m_FilePNGIcon;
				icon = Renderer::m_SceneData.m_ResourceManager.getTexture(itemPath);
			}
			else if (GetFileExtension(directoryEntry) == "jpg")
			{
				//icon = m_FileJPGIcon;
				icon = Renderer::m_SceneData.m_ResourceManager.getTexture(itemPath);
			}
			else
			{
				icon = m_FileOtherIcon;
			}

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->GetID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Delete"))
				{
					std::cout << filenameString.c_str() << std::endl;
				}
				ImGui::EndPopup();
			}

			if (ImGui::BeginDragDropSource())
			{
				std::filesystem::path relativePath(path);
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();

			}
			size_t lastindex = filenameString.find_last_of(".");
			std::string fileName = filenameString.substr(0, lastindex);
			ImGui::TextWrapped(fileName.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::End();
	}

	const std::string ContentBrowserPanel::GetFileExtension(std::filesystem::directory_entry e)
	{
		const std::filesystem::path p = e.path();
		const std::string s_path = p.string();

		const size_t slash = s_path.find_last_of("/\\");
		const std::string m_SelectedFile = s_path.substr(slash + 1);

		const std::string extension = m_SelectedFile.substr(m_SelectedFile.find_last_of(".") + 1);

		return extension;
	}
}