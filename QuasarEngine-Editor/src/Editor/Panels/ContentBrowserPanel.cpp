#include "ContentBrowserPanel.h"

#include <iostream>

#include "imgui.h"
#include "Editor/Importer/TextureConfigImporter.h"
#include "Editor/Importer/TextureImporter.h"

#include <QuasarEngine/Renderer/Renderer.h>
#include <QuasarEngine/Asset/Asset.h>

#include <QuasarEngine/Core/Log.h>

namespace QuasarEngine
{
	ContentBrowserPanel::ContentBrowserPanel(const std::string& projectPath, AssetImporter* importer) : m_BaseDirectory(projectPath + "\\Assets"), m_CurrentDirectory(m_BaseDirectory), m_AssetImporter(importer)
	{
		TextureSpecification spec;
		spec.flip = true;
		spec.alpha = true;

		m_DirectoryIcon = Texture2D::CreateTexture2D("Assets/Icons/texture_dossier.png", spec);
		m_FilePNGIcon = Texture2D::CreateTexture2D("Assets/Icons/texture_png.png", spec);
		m_FileJPGIcon = Texture2D::CreateTexture2D("Assets/Icons/texture_jpg.png", spec);
		m_FileOBJIcon = Texture2D::CreateTexture2D("Assets/Icons/texture_obj.png", spec);
		m_FileOtherIcon = Texture2D::CreateTexture2D("Assets/Icons/texture_texte.png", spec);
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

			std::string extension = std::filesystem::path(directoryEntry).extension().string();

			if (extension == ".ultconf")
			{
				continue;
			}

			ImGui::PushID(filenameString.c_str());

			std::shared_ptr<Texture2D> icon;


			AssetType fileType = Renderer::m_SceneData.m_AssetManager->getTypeFromExtention(extension);

			if (directoryEntry.is_directory())
			{
				icon = m_DirectoryIcon;
			}
			else if (fileType == AssetType::MESH)
			{
				icon = m_FileOBJIcon;
			}
			else if (fileType == AssetType::TEXTURE)
			{
				if (Renderer::m_SceneData.m_AssetManager->isAssetLoaded(itemPath))
				{
					icon = Renderer::m_SceneData.m_AssetManager->getAsset<Texture2D>(itemPath);
				}
				else
				{
					TextureSpecification spec = TextureConfigImporter::ImportTextureConfig(itemPath);
					std::shared_ptr<Texture2D> texture = Texture2D::CreateTexture2D(itemPath, spec);
					Renderer::m_SceneData.m_AssetManager->loadAsset(itemPath, texture);

					if (Renderer::m_SceneData.m_AssetManager->isAssetLoaded(itemPath))
					{
						icon = Renderer::m_SceneData.m_AssetManager->getAsset<Texture2D>(itemPath);
					}
					else
					{
						icon = m_FileOtherIcon;
					}
				}
			}
			else if (fileType == AssetType::QASSET)
			{
				if (Renderer::m_SceneData.m_AssetManager->isAssetRegistered(itemPath))
				{
					AssetType type = Renderer::m_SceneData.m_AssetManager->getAssetType(itemPath);
					switch (type)
					{
					case AssetType::TEXTURE:
						if (Renderer::m_SceneData.m_AssetManager->isAssetLoaded(itemPath))
						{
							icon = Renderer::m_SceneData.m_AssetManager->getAsset<Texture2D>(itemPath);
						}
						else
						{
							std::shared_ptr<Texture2D> texture = TextureImporter::importTexture(itemPath);
							if (texture)
							{
								Renderer::m_SceneData.m_AssetManager->loadAsset(itemPath, texture);
								icon = Renderer::m_SceneData.m_AssetManager->getAsset<Texture2D>(itemPath);
							}
							else
							{
								icon = m_FileOtherIcon;
							}
						}
						break;
					default:
						icon = m_FileOtherIcon;
						break;
					}
				}
				else
				{
					icon = m_FileOtherIcon;
				}
			}
			else
			{
				icon = m_FileOtherIcon;
			}

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->GetID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginPopupContextItem())
			{
				if (Renderer::m_SceneData.m_AssetManager->getAssetType(directoryEntry.path().string()) == AssetType::TEXTURE)
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
				else if (Renderer::m_SceneData.m_AssetManager->getAssetType(directoryEntry.path().string()) == AssetType::TEXTURE)
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
}