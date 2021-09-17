#include "DIPLayer.h"
#include "DIP/DIP/DIP.h"

#include <Editor/basic.h>

namespace Chaf
{
	DIPLayer::DIPLayer()
	{
	}

	void DIPLayer::OnAttach()
	{
		m_source = CreateRef<Image>("../assets/texture/back.jpg");
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
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		show_texture(m_source, "Source", false);

		show_texture(m_target, "Target", false);


	}

	void DIPLayer::OnEvent(Event& event)
	{
	}

	void DIPLayer::show_texture(Ref<Image>& image, const char* label, bool file_dialog)
	{
		if (!image)
		{
			return;
		}
		
		ImGui::Begin(label);
		ImGui::Image((void*)image->getTextureID(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
			ImGui::OpenPopup(label);
		if (ImGui::BeginPopup(label))
		{
			ImGui::Image((void*)image->getTextureID(), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
			EditorBasic::SetPopupFlag("Select Image");
			ImGui::EndPopup();
		}
		EditorBasic::GetFileDialog("Select Image", ".png,.jpg,.bmp,.jpeg", [=](const std::string& path) {CHAF_INFO(path); });
		ImGui::End();
	}
}