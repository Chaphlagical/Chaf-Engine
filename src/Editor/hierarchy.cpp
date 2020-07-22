#include <Editor/hierarchy.h>
#include <Scene/scene_layer.h>
#include <imgui.h>

namespace Chaf
{
	uint32_t Hierarchy::m_SelectIndex = 0;
	bool Hierarchy::m_Focus = false;

	void Hierarchy::ShowHierachy()
	{
		ImGui::Begin("Hierarchy");
		auto names = SceneLayer::GetInstance()->GetMeshNames();
		if(ImGui::CollapsingHeader("Scene"))
		{
			for (auto name : names)
			{
				if (ImGui::TreeNodeEx(name.first.c_str()))
				{
					ImGui::SetKeyboardFocusHere();
					ImGui::TreePop();
				}
				if (ImGui::IsItemDeactivated())
					SceneLayer::GetInstance()->SetSelectMesh(name.first);
			}
		}
		ImGui::End();
	}

}