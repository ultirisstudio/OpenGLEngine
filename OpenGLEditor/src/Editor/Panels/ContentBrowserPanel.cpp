#include "ContentBrowserPanel.h"

#include <OpenGLEngine/Renderer/Renderer.h>

#include "imgui.h"

namespace OpenGLEngine
{
	ContentBrowserPanel::ContentBrowserPanel(const std::string& projectPath) : m_BaseDirectory(projectPath + "\\Assets"), m_CurrentDirectory(m_BaseDirectory)
	{
		TextureSpecification spec;
		spec.flip = true;
		spec.alpha = true;

		m_DirectoryIcon = Texture::CreateTexture("Assets/Icons/texture_dossier.png", spec);
		m_FilePNGIcon = Texture::CreateTexture("Assets/Icons/texture_png.png", spec);
		m_FileJPGIcon = Texture::CreateTexture("Assets/Icons/texture_jpg.png", spec);
		m_FileOBJIcon = Texture::CreateTexture("Assets/Icons/texture_obj.png", spec);
		m_FileOtherIcon = Texture::CreateTexture("Assets/Icons/texture_texte.png", spec);
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		if (m_TextureViewerPanel)
		{
			if (!m_TextureViewerPanel->IsOpen())
				m_TextureViewerPanel.reset();
			else
				m_TextureViewerPanel->OnImGuiRender();
		}

		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != std::filesystem::path(m_BaseDirectory))
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
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
			std::string extension = GetFileExtension(directoryEntry);

			if (directoryEntry.is_directory())
			{
				icon = m_DirectoryIcon;
			}
			else if (extension == "obj")
			{
				icon = m_FileOBJIcon;
			}
			else if (extension == "png" || extension == "jpg")
			{
				//TODO : load texture config file

				if (Renderer::m_SceneData.m_ResourceManager.GetTexture(itemPath))
				{
					icon = Renderer::m_SceneData.m_ResourceManager.GetTexture(itemPath);
				}
				else
				{
					TextureSpecification spec;
					icon = Renderer::m_SceneData.m_ResourceManager.CreateTexture(itemPath, spec);
				}
			}
			else
			{
				icon = m_FileOtherIcon;
			}

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->GetID(), {thumbnailSize, thumbnailSize}, {0, 1}, {1, 0});

			if (ImGui::BeginPopupContextItem())
			{
				if (extension == "png" || extension == "jpg")
				{
					if (ImGui::MenuItem("Modify"))
					{
						m_TextureViewerPanel = std::make_shared<TextureViewerPanel>(relativePath);
					}

					ImGui::Separator();
				}

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
				else if (extension == "png" || extension == "jpg")
				{
					m_TextureViewerPanel = std::make_shared<TextureViewerPanel>(relativePath);
				}
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