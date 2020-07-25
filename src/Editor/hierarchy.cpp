/*#include <Editor/hierarchy.h>
#include <Editor/menu.h>
#include <Scene/scene_layer.h>
#include <imgui.h>

namespace Chaf
{
	uint32_t Hierarchy::m_SelectIndex = 0;
	bool Hierarchy::m_Focus = false;
	char* Hierarchy::m_Buf = new char[50];
	std::string Hierarchy::m_SelectName = "";

	void Hierarchy::ShowHierachy()
	{
		ImGui::Begin("Hierarchy");
		auto names = SceneLayer::GetInstance()->GetMeshNames();
		if(ImGui::CollapsingHeader("Scene"))
		{
			for (auto &name : names)
			{
				ImGui::SetKeyboardFocusHere();
				if (ImGui::TreeNodeEx(name.first.c_str())) ImGui::TreePop();
				if (ImGui::IsItemDeactivated())
					SceneLayer::GetInstance()->SetSelectMesh(name.first);

				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
				{
					for (int i = 0; i < name.first.length(); i++)
						m_Buf[i] = name.first[i];
					m_Buf[name.first.length()] = '\0';
					m_SelectName = name.first;
					ImGui::OpenPopup("item_popup");
				}
			}
			if (ImGui::BeginPopup("item_popup"))
			{
				if (ImGui::BeginMenu("New"))
				{
					Menu::ShowAddObjectMenuBegin();
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Delete"))
				{
					SceneLayer::GetInstance()->PopMesh(m_SelectName);
					ImGui::CloseCurrentPopup();
				}
				ImGui::Separator();
				ImGui::Text("Rename:");
				ImGui::InputText("##edit", m_Buf, 50);
				if (m_SelectName != m_Buf && ImGui::Button("OK"))
				{
					SceneLayer::GetInstance()->RenameMesh(m_SelectName, m_Buf);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right) 
			&& SceneLayer::GetInstance()->GetMeshNumber() == 0)
		{
			ImGui::OpenPopup("window_popup");
		}

		if (ImGui::BeginPopup("window_popup"))
		{
			ImGui::Text("New Object");
			ImGui::Separator();
			Menu::ShowAddObjectMenuBegin();
			ImGui::EndPopup();
		}
		ImGui::End();
	}

}
*/