#include "DIPLayer.h"
#include "DIP/DIP/DIP.h"

#include <Editor/basic.h>

namespace Chaf
{
	inline void show_texture(GLuint texture_id, uint32_t width, uint32_t height)
	{
		ImGui::Image((void*)texture_id, ImVec2((float)width, (float)height), ImVec2(0, 1), ImVec2(1, 0));
		/*if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
			ImGui::OpenPopup(texture_id);
		if (ImGui::BeginPopup(texture_id))
		{
			ImGui::Image((void*)texture_id, ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
			func();
			ImGui::EndPopup();
		}*/
	}

	DIPLayer::DIPLayer()
	{
	}

	void DIPLayer::OnAttach()
	{
		m_source = CreateRef<Image>("D://Workspace//Chaf-Engine//assets//texture//768d35396583f38b1136a6d45b94cf7c.jpg");
	}

	void DIPLayer::OnDetach()
	{
	}

	void DIPLayer::OnUpdate(Timestep timestep)
	{
	}

	void DIPLayer::OnImGuiRender()
	{
		ImGui::Begin("Source");
		show_texture(m_source->getTextureID(),ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
		ImGui::End();
	}

	void DIPLayer::OnEvent(Event& event)
	{
	}
}