#include <Editor/component.h>
#include <Editor/FileDialog/ImGuiFileDialog.h>
#include <Scene/scene_layer.h>
#include <Editor/hierarchy.h>

#include <imgui.h>

namespace Chaf
{
	void Component::ShowComponent()
	{
		ImGui::Begin("Component");

		if (SceneLayer::GetInstance()->IsSelectValid())
		{
			Ref<TriMesh> mesh = SceneLayer::GetInstance()->GetSelectMesh();
			ImGui::Text(mesh->GetName().c_str());
			ImGui::Separator();
			if (ImGui::CollapsingHeader("Transform"))
			{
				glm::vec3 position = mesh->GetPosition();
				ImGui::DragFloat3("Position", (float*)&position, 0.1f);
				mesh->SetPosition(position);
				glm::vec3 rotaion = mesh->GetRotation();
				ImGui::DragFloat3("Rotation", (float*)&rotaion, 1.0f);
				mesh->SetRotation(rotaion);
				glm::vec3 scale = mesh->GetScale();
				ImGui::DragFloat3("Scale", (float*)&scale, 0.01f);
				mesh->SetScale(scale);
			}
			if (ImGui::CollapsingHeader("Texture"))
			{
				ImGui::Text("Color");
				glm::vec4 color = mesh->GetColor();
				ImGui::ColorEdit4("color", (float*)&(color));
				mesh->SetColor(color);
				ImGui::Separator();
				ImGui::Columns(2, "Default Texture");
				ImGui::Text("Default Texture");
				if (mesh->HasTexture())
					ImGui::Image((void*)mesh->GetTexture()->GetRendererID(), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
				else
					ImGui::Image((void*)SceneLayer::GetInstance()->GetDefaultDisplayTexture()->GetRendererID(), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
				ImGui::NextColumn();
				ImGui::NewLine();
				if (ImGui::MenuItem("Load"))
				{
					igfd::ImGuiFileDialog::Instance()->OpenDialog("Choose Texture Image", "Choose File", ".png\0.jpg\0.bmp\0.jpeg", ".");
				}
				if (igfd::ImGuiFileDialog::Instance()->FileDialog("Choose Texture Image"))
				{
					if (igfd::ImGuiFileDialog::Instance()->IsOk == true)
					{
						std::string filePathName = igfd::ImGuiFileDialog::Instance()->GetFilepathName();
						mesh->SetTexture(filePathName);
					}
					igfd::ImGuiFileDialog::Instance()->CloseDialog("Choose Texture Image");
				}
				ImGui::NewLine();
				if (ImGui::MenuItem("Reset"))mesh->ResetTexture();
				ImGui::NewLine();
				ImGui::MenuItem("Add");
				ImGui::Columns(1);
			}
		}
		ImGui::End();
	}
}