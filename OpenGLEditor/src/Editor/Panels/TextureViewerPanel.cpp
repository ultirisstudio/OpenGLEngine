#include "TextureViewerPanel.h"

#include <iostream>
#include <imgui.h>

#include <OpenGLEngine/Renderer/Renderer.h>

namespace OpenGLEngine
{
	namespace Utils
	{
		static const char* TextureWrapToChar(TextureWrap wrap)
		{
			switch (wrap)
			{
			case TextureWrap::REPEAT: return "Repeat";
			case TextureWrap::MIRRORED_REPEAT: return "Mirrored Repeat";
			case TextureWrap::CLAMP_TO_EDGE: return "Clamp to Edge";
			case TextureWrap::CLAMP_TO_BORDER: return "Clamp to Border";
			}
			return "Unknown";
		}

		static const char* TextureFilterToChar(TextureFilter filter)
		{
			switch (filter)
			{
			case TextureFilter::NEAREST: return "Nearest";
			case TextureFilter::LINEAR: return "Linear";
			case TextureFilter::NEAREST_MIPMAP_NEAREST: return "Nearest Mipmap Nearest";
			case TextureFilter::LINEAR_MIPMAP_NEAREST: return "Linear Mipmap Nearest";
			case TextureFilter::NEAREST_MIPMAP_LINEAR: return "Nearest Mipmap Linear";
			case TextureFilter::LINEAR_MIPMAP_LINEAR: return "Linear Mipmap Linear";
			}
			return "Unknown";
		}

		static TextureWrap CharToTextureWrap(const char* wrap)
		{
			if (strcmp(wrap, "Repeat") == 0) return TextureWrap::REPEAT;
			if (strcmp(wrap, "Mirrored Repeat") == 0) return TextureWrap::MIRRORED_REPEAT;
			if (strcmp(wrap, "Clamp to Edge") == 0) return TextureWrap::CLAMP_TO_EDGE;
			if (strcmp(wrap, "Clamp to Border") == 0) return TextureWrap::CLAMP_TO_BORDER;
			return TextureWrap::REPEAT;
		}

		static TextureFilter CharToTextureFilter(const char* filter)
		{
			if (strcmp(filter, "Nearest") == 0) return TextureFilter::NEAREST;
			if (strcmp(filter, "Linear") == 0) return TextureFilter::LINEAR;
			if (strcmp(filter, "Nearest Mipmap Nearest") == 0) return TextureFilter::NEAREST_MIPMAP_NEAREST;
			if (strcmp(filter, "Linear Mipmap Nearest") == 0) return TextureFilter::LINEAR_MIPMAP_NEAREST;
			if (strcmp(filter, "Nearest Mipmap Linear") == 0) return TextureFilter::NEAREST_MIPMAP_LINEAR;
			if (strcmp(filter, "Linear Mipmap Linear") == 0) return TextureFilter::LINEAR_MIPMAP_LINEAR;
			return TextureFilter::NEAREST;
		}
	}

	TextureViewerPanel::TextureViewerPanel(std::filesystem::path path) : m_TexturePath(path)
	{
		m_Texture = Renderer::m_SceneData.m_ResourceManager.GetTexture(m_TexturePath.string());
		m_Specification = m_Texture->GetSpecification();
	}

	void TextureViewerPanel::OnImGuiRender()
	{
		std::string name = m_TexturePath.filename().string() + " (Texture Viewer)";
		ImGui::Begin(name.c_str());

		ImGui::Columns(2, 0, true);

		if (m_Texture)
		{
			ImGui::Image((void*)m_Texture->GetID(), ImVec2{ 512, 512 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		}
		else
		{
			ImGui::Text("Texture not found");
		}

		ImGui::NextColumn();

		if (m_Texture)
		{
			ImGui::Text("Width: %d", m_Specification.width); ImGui::SameLine();
			ImGui::Text("Height: %d", m_Specification.height);
			ImGui::Text("Channels: %d", m_Specification.channels);

			ImGui::Separator();

			ImGui::Checkbox("Alpha", &m_Specification.alpha);
			ImGui::Checkbox("Gamma", &m_Specification.gamma);
			ImGui::Checkbox("Flip Vertically", &m_Specification.flip);

			const char* wrap_items[] = { "Repeat", "Mirrored Repeat", "Clamp to Edge", "Clamp to Border" };
			const char* filter_items[] = { "Nearest", "Linear", "Nearest Mipmap Nearest", "Linear Mipmap Nearest", "Nearest Mipmap Linear", "Linear Mipmap Linear" };

			const char* wrap_r_current_item = Utils::TextureWrapToChar(m_Specification.wrap_r);
			const char* wrap_s_current_item = Utils::TextureWrapToChar(m_Specification.wrap_s);
			const char* wrap_t_current_item = Utils::TextureWrapToChar(m_Specification.wrap_t);

			const char* min_filter_current_item = Utils::TextureFilterToChar(m_Specification.min_filter_param);
			const char* mag_filter_current_item = Utils::TextureFilterToChar(m_Specification.mag_filter_param);

			if (ImGui::BeginCombo("Wrap R", wrap_r_current_item))
			{
				for (int n = 0; n <= 3; n++)
				{
					bool is_selected = (wrap_r_current_item == wrap_items[n]);
					if (ImGui::Selectable(wrap_items[n], is_selected))
					{
						m_Specification.wrap_r = Utils::CharToTextureWrap(wrap_items[n]);
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (ImGui::BeginCombo("Wrap S", wrap_s_current_item))
			{
				for (int n = 0; n <= 3; n++)
				{
					bool is_selected = (wrap_s_current_item == wrap_items[n]);
					if (ImGui::Selectable(wrap_items[n], is_selected))
					{
						m_Specification.wrap_s = Utils::CharToTextureWrap(wrap_items[n]);
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (ImGui::BeginCombo("Wrap T", wrap_t_current_item))
			{
				for (int n = 0; n <= 3; n++)
				{
					bool is_selected = (wrap_t_current_item == wrap_items[n]);
					if (ImGui::Selectable(wrap_items[n], is_selected))
					{
						m_Specification.wrap_t = Utils::CharToTextureWrap(wrap_items[n]);
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (ImGui::BeginCombo("Min Filter", min_filter_current_item))
			{
				for (int n = 0; n <= 5; n++)
				{
					bool is_selected = (min_filter_current_item == filter_items[n]);
					if (ImGui::Selectable(filter_items[n], is_selected))
					{
						m_Specification.min_filter_param = Utils::CharToTextureFilter(filter_items[n]);
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (ImGui::BeginCombo("Mag Filter", mag_filter_current_item))
			{
				for (int n = 0; n <= 5; n++)
				{
					bool is_selected = (mag_filter_current_item == filter_items[n]);
					if (ImGui::Selectable(filter_items[n], is_selected))
					{
						m_Specification.mag_filter_param = Utils::CharToTextureFilter(filter_items[n]);
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (ImGui::Button("Apply"))
			{
				m_Texture = Renderer::m_SceneData.m_ResourceManager.UpdateTexture(m_TexturePath.string(), m_Specification);

				//TODO : Update config texture file
			}

			if (ImGui::Button("Close"))
			{
				m_IsOpen = false;
			}
		}

		ImGui::End();
	}
}