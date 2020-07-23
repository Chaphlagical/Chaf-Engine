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
				ImGui::Text("Default Texture");
				for (int i = 0; i < mesh->GetTextureNum(); i++)
				{
					ImGui::Columns(2, "Default Texture");
					ImGui::Text(("Index: " + std::to_string(i + 1)).c_str());
					if (mesh->HasTexture(i))
						ImGui::Image((void*)mesh->GetTexture(i)->GetRendererID(), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
					else
						ImGui::Image((void*)SceneLayer::GetInstance()->GetDefaultDisplayTexture()->GetRendererID(), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
					ImGui::NextColumn();
					ImGui::PushID(i);
					if (ImGui::MenuItem("Load"))
					{
						igfd::ImGuiFileDialog::Instance()->OpenDialog("Choose Texture" + std::to_string(i), "Choose File", ".png,.jpg,.bmp,.jpeg", ".");
					}
					if (igfd::ImGuiFileDialog::Instance()->FileDialog("Choose Texture"+ std::to_string(i)))
					{
						if (igfd::ImGuiFileDialog::Instance()->IsOk == true)
						{
							std::string filePathName = igfd::ImGuiFileDialog::Instance()->GetFilepathName();
							mesh->SetTexture(filePathName, i);
						}
						igfd::ImGuiFileDialog::Instance()->CloseDialog("Choose Texture" + std::to_string(i));
					}
					ImGui::PopID();
					ImGui::PushID(i+2);
					if (ImGui::MenuItem("Reset"))mesh->ResetTexture(i);
					ImGui::PopID();
					if (i == 0)
					{
						if (ImGui::MenuItem("Add"))
							mesh->AddTexture();
					}
					else
					{
						if (ImGui::MenuItem("Delete"))
						{
							mesh->DeleteTexture(i);	i = 0;
						}
					}
					float weight = mesh->GetTextureWeight(i);
					ImGui::PushID(i + 3);
					ImGui::SliderFloat("weight", &weight, 0.0f, 1.0f, "%.2f");
					mesh->SetTextureWeight(i, weight);
					ImGui::PopID();
					ImGui::Columns(1);
					ImGui::NewLine();
				}
			}
		}
		ImGui::End();
	}
}