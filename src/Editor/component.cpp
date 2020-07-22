#include <Editor/component.h>
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

			int linemode = static_cast<int>(SceneLayer::GetInstance()->GetLineMode());
			ImGui::Combo("Raster", &linemode, "Polygon\0Wireframe");
			SceneLayer::GetInstance()->SetLineMode(static_cast<bool>(linemode));
			ImGui::SameLine();
			bool gridFlag = SceneLayer::GetInstance()->IsShowGrid();
			ImGui::Checkbox("Grid", &gridFlag);
			SceneLayer::GetInstance()->SetShowGrid(gridFlag);

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
				ImGui::Text("Default: "); ImGui::SameLine();
				if (mesh->HasTexture())
					ImGui::Image((void*)mesh->GetTexture()->GetRendererID(), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
				else
					ImGui::Image((void*)SceneLayer::GetInstance()->GetDefaultDisplayTexture()->GetRendererID(), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
				ImGui::SameLine();

				ImGui::Button("Load");
				ImGui::SameLine();
				if (ImGui::Button("Reset"))mesh->ResetTexture();
			}
		}
		ImGui::End();
	}
}