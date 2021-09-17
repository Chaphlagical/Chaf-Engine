#include "DIPLayer.h"
#include "DIP/DIP/DIP.h"

#include "ColorTransfer.h"

#include <Editor/basic.h>

namespace Chaf
{
	DIPLayer::DIPLayer()
	{
	}

	void DIPLayer::OnAttach()
	{
		m_source = CreateRef<Image>("../assets/texture/768d35396583f38b1136a6d45b94cf7c.jpg");
		m_target = CreateRef<Image>("../assets/texture/613934.jpg");
	}

	void DIPLayer::OnDetach()
	{
	}

	void DIPLayer::OnUpdate(Timestep timestep)
	{
	}

	void DIPLayer::OnImGuiRender()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Load Source"))
				{
					EditorBasic::SetPopupFlag("Select Source Image");
				}
				if (ImGui::MenuItem("Load Target"))
				{
					EditorBasic::SetPopupFlag("Select Target Image");
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Process"))
			{
				if (m_source && m_target)
				{
					if (ImGui::MenuItem("Color Transfer"))
					{
						m_result.reset();
						ColorTransfer transfer(m_source, m_target);
						m_result = transfer.solve();
					}
					if (ImGui::MenuItem("Colorization"))
					{
						m_result.reset();
						m_result = CreateRef<Image>(m_source->getImage());
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		EditorBasic::GetFileDialog("Select Source Image", ".png,.jpg,.bmp,.jpeg", [this](const std::string& path) {
			CHAF_INFO(path);
			m_source.reset();
			m_source = CreateRef<Image>(path);
			});

		EditorBasic::GetFileDialog("Select Target Image", ".png,.jpg,.bmp,.jpeg", [this](const std::string& path) {
			CHAF_INFO(path);
			m_target.reset();
			m_target = CreateRef<Image>(path);
			});

		m_has_source = m_source != nullptr;
		m_has_target = m_target != nullptr;
		m_has_result = m_result != nullptr;

		show_texture(m_source, "Source", true, m_has_source);
		show_texture(m_target, "Target", true, m_has_target);
		show_texture(m_result, "Result", false, m_has_result);

		if (!m_has_source)
		{
			m_source.reset();
			m_source = nullptr;
		}

		if (!m_has_target)
		{
			m_target.reset();
			m_target = nullptr;
		}

		if (!m_has_result)
		{
			m_result.reset();
			m_result = nullptr;
		}
	}

	void DIPLayer::OnEvent(Event& event)
	{
	}

	void DIPLayer::show_texture(Ref<Image>& image, const char* label, bool file_dialog, bool& handle)
	{
		if (!image)
		{
			return;
		}

		ImGui::Begin(label, &handle);
		ImGui::Image((void*)image->getTextureID(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

		if (file_dialog)
		{
			if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
				ImGui::OpenPopup(label);
			if (ImGui::BeginPopup(label))
			{
				ImGui::Image((void*)image->getTextureID(), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
				EditorBasic::SetPopupFlag("Select Image");
				ImGui::EndPopup();
			}
			EditorBasic::GetFileDialog("Select Image", ".png,.jpg,.bmp,.jpeg", [&image](const std::string& path) {
				CHAF_INFO(path);
				image.reset();
				image = CreateRef<Image>(path);
				});
		}

		ImGui::End();
	}
}