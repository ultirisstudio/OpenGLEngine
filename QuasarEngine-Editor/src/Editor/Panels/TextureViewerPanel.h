#pragma once

#include <filesystem>

#include <QuasarEngine/Resources/Texture.h>

namespace QuasarEngine
{
	class TextureViewerPanel
	{
	public:
		TextureViewerPanel(std::filesystem::path path);
		~TextureViewerPanel() = default;

		bool IsOpen() { return m_IsOpen; }

		void OnImGuiRender();
	private:
		std::filesystem::path m_TexturePath;

		std::shared_ptr<Texture> m_Texture;
		TextureSpecification m_Specification;

		bool m_IsOpen = true;
	};
}