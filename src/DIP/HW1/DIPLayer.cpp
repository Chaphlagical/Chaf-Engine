#include "DIPLayer.h"
#include "DIP/DIP/DIP.h"

#include "ColorTransfer.h"
#include "Colorization.h"

#include <Editor/basic.h>

namespace Chaf
{
	DIPLayer::DIPLayer()
	{
	}

	void DIPLayer::OnAttach()
	{
		m_source = CreateRef<Image>("../assets/texture/Colorization/source/1.jpg");
		m_target = CreateRef<Image>("../assets/texture/Colorization/target/1.jpg");
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
						Colorization colorization(m_source, m_target);
						colorization.setMethod(ColorizationMethod::Global);
						m_result = colorization.solve();
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		EditorBasic::GetFileDialog("Select Source Image", ".png,.jpg,.bmp,.jpeg", [this](const std::string& path) {
			if (!m_source)
			{
				m_source = CreateRef<Image>(path);
				m_result.reset();
				m_result = nullptr;
			}
			else if (m_source->getPath() != path)
			{
				m_source.reset();
				m_source = CreateRef<Image>(path);
				m_result.reset();
				m_result = nullptr;
			}
			});

		EditorBasic::GetFileDialog("Select Target Image", ".png,.jpg,.bmp,.jpeg", [this](const std::string& path) {
			if (!m_target)
			{
				m_target = CreateRef<Image>(path);
				m_result.reset();
				m_result = nullptr;
			}
			else if (m_target->getPath() != path)
			{
				m_target.reset();
				m_target = CreateRef<Image>(path);
				m_result.reset();
				m_result = nullptr;
			}
			});

		show_texture(m_source, "Source", false, true);
		show_texture(m_target, "Target", false, true);
		show_texture(m_result, "Result", false, true);
	}

	void DIPLayer::OnEvent(Event& event)
	{
	}

	void DIPLayer::show_texture(Ref<Image>& image, const char* label, bool file_dialog, bool roi)
	{
		if (!image)
		{
			return;
		}

		ImGui::Begin(label);
		ImGui::Image((void*)image->getTextureID(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
	}
}