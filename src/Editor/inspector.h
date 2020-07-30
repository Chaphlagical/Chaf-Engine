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
		static bool DeleteComponent(const std::string& label)
		{
			if (ImGui::BeginPopupContextItem(("Delete "+label).c_str()))
			{
				if (ImGui::MenuItem("Delete Component"))
				{
					EditorBasic::GetSelectEntity().RemoveComponent<T>();
					ImGui::End();
					return true;
				}
				ImGui::EndPopup();
			}
			return false;
		}

		template<typename T>
		static void ShowSetTexture(Ref<Texture2D>& texture, const std::string& label, T& handler)
		{
			ImGui::Columns(2, (label + " Texture").c_str());
			ImGui::Text((label + " Texture").c_str());
			EditorBasic::ShowTexture((label + " preview").c_str(), texture);

			ImGui::NextColumn();
			ImGui::NewLine();
			if (ImGui::MenuItem("Show Path"))
				ImGui::OpenPopup((label + " Path").c_str());
			if (ImGui::BeginPopup((label + " Path").c_str()))
			{
				ImGui::Text((texture->GetPathName()).c_str());
				ImGui::EndPopup();
			}
			ImGui::NewLine();
			ImGui::PushID(std::to_string(EditorBasic::GetSelectEntity().ID()).c_str());
			if (ImGui::MenuItem("Load"))
				EditorBasic::SetPopupFlag(("Choose "+label).c_str());
			EditorBasic::GetFileDialog(("Choose " + label).c_str(), ".png,.jpg,.bmp,.jpeg", [&](const std::string& filePathName) {
				handler->ResetTexture(texture, filePathName);
			});
			ImGui::PopID();
			ImGui::PushID(std::to_string(EditorBasic::GetSelectEntity().ID()).c_str());
			ImGui::NewLine();
			if (ImGui::MenuItem("Reset"))handler->ResetTexture(texture);
			ImGui::Columns(1);
			ImGui::PopID();
		}
	};
}