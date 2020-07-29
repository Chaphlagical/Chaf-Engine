#pragma once

#include <Engine/core.h>
#include <Scene/components.h>
#include <Editor/basic.h>
#include <Renderer/texture.h>
#include <imgui.h>

namespace Chaf
{
	class CHAF_API Inspector
	{
	public:
		static void ShowInspector(bool* p_open);

		static void ShowTransformComponent();
		static void ShowMaterialComponent();
		static void ShowMeshComponent();
		static void ShowLightComponent();

		static std::string BoolString(bool x) { if (x)return "true"; return "false"; }

		template<typename T>
		static void DeleteComponent(const std::string& label)
		{
			if (ImGui::BeginPopupContextItem(("Delete "+label).c_str()))
			{
				if (ImGui::MenuItem("Delete Component"))
				{
					EditorBasic::GetSelectEntity().RemoveComponent<T>();
					ImGui::End();
					return;
				}
				ImGui::EndPopup();
			}
		}
	};
}