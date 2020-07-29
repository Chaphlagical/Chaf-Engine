#include <Editor/inspector.h>
#include <Editor/FileDialog/ImGuiFileDialog.h>

namespace Chaf
{
	void Inspector::ShowInspector(bool* p_open)
	{
		ImGui::Begin("Inspector", p_open);
		if (EditorBasic::GetSelectEntity() && EditorBasic::GetSelectEntity().HasComponent<TagComponent>() && ! EditorBasic::GetSelectEntity().IsRoot())
		{
			std::vector<std::string> addComponentItem;
			ShowTransformComponent();
			if (EditorBasic::GetSelectEntity().HasComponent<MeshComponent>())
				ShowMeshComponent();
			else addComponentItem.push_back("Mesh Component");

			if (EditorBasic::GetSelectEntity().HasComponent<MaterialComponent>())
				ShowMaterialComponent();
			else addComponentItem.push_back("Material Component");

			if (EditorBasic::GetSelectEntity().HasComponent<LightComponent>())
				ShowLightComponent();
			else addComponentItem.push_back("Light Component");

			if (addComponentItem.size() > 0)
			{
				ImGui::NewLine();
				ImGui::Separator();
				if (ImGui::Button("Add Component", { ImGui::GetWindowWidth(),50.0f }))
					ImGui::OpenPopup("Add Component");
			}
			if (ImGui::BeginPopup("Add Component"))
			{
				for (auto component : addComponentItem)
					if (ImGui::MenuItem(component.c_str()))
					{
						if (component == "Mesh Component" && !EditorBasic::GetSelectEntity().HasComponent<MeshComponent>())
							EditorBasic::GetSelectEntity().AddComponent<MeshComponent>();
						else if (component == "Material Component" && !EditorBasic::GetSelectEntity().HasComponent<MaterialComponent>())
							EditorBasic::GetSelectEntity().AddComponent<MaterialComponent>();
						else if (component == "Light Component" && !EditorBasic::GetSelectEntity().HasComponent<LightComponent>())
							EditorBasic::GetSelectEntity().AddComponent<LightComponent>();
					}
				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}

	void Inspector::ShowTransformComponent()
	{
		ImGui::Text(EditorBasic::GetSelectEntity().GetComponent<TagComponent>().Tag.c_str());
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::DragFloat3("Position", (float*)&EditorBasic::GetSelectEntity().GetComponent<TransformComponent>().Position, 0.1f);
			ImGui::DragFloat3("Rotation", (float*)&EditorBasic::GetSelectEntity().GetComponent<TransformComponent>().Rotation, 1.0f);
			ImGui::DragFloat3("Scale", (float*)&EditorBasic::GetSelectEntity().GetComponent<TransformComponent>().Scale, 0.01f);
			ImGui::Separator();
		}
	}

	void Inspector::ShowMaterialComponent()
	{
		if (ImGui::CollapsingHeader("Material"))
		{
			DeleteComponent<MaterialComponent>("Material");

			//	
			const char* materialItems[] = { "None", "Emisson", "Phong" };
			auto tmp = EditorBasic::GetSelectEntity().GetComponent<MaterialComponent>().Type;
			auto& material = EditorBasic::GetSelectEntity().GetComponent<MaterialComponent>();
			ImGui::Combo("Type", (int*)(&material.Type), materialItems, IM_ARRAYSIZE(materialItems));
			if (tmp != material.Type)
				material.ResetType();
			switch (material.Type)
			{
			case MaterialType::Material_Emission:
			{
				ImGui::ColorEdit3("Emission Color", (float*)(&CastRef<EmissionMaterial>(material.MaterialSrc)->EmissionColor));
				ImGui::SliderFloat("Intensity", (float*)(&CastRef<EmissionMaterial>(material.MaterialSrc)->Intensity), 0.0f, 1.0f);
				break;
			}

			case MaterialType::Material_Phong:
			{
				ImGui::ColorEdit3("Color", (float*)(&CastRef<PhongMaterial>(material.MaterialSrc)->Color));
				ImGui::DragFloat("Shininess", (float*)(&CastRef<PhongMaterial>(material.MaterialSrc)->Shininess));
				ImGui::Separator();

				ImGui::Columns(2, "Diffuse Texture");
				ImGui::Text("Diffuse Texture");
				auto& phongMaterial = CastRef<PhongMaterial>(EditorBasic::GetSelectEntity().GetComponent<MaterialComponent>().MaterialSrc);
				EditorBasic::ShowTexture("diffuse preview", phongMaterial->DiffuseTexture);

				ImGui::NextColumn();
				ImGui::NewLine();
				if (ImGui::MenuItem("Show Path"))
					ImGui::OpenPopup("diffuse Path");
				if (ImGui::BeginPopup("diffuse Path"))
				{
					ImGui::Text((phongMaterial->DiffuseTexture->GetPathName()).c_str());
					ImGui::EndPopup();
				}
				ImGui::NewLine();
				ImGui::PushID(std::to_string(EditorBasic::GetSelectEntity().ID()).c_str());
				if (ImGui::MenuItem("Load"))
					EditorBasic::SetPopupFlag("Choose Diffuse");
				EditorBasic::GetFileDialog("Choose Diffuse", ".png,.jpg,.bmp,.jpeg", [&](const std::string& filePathName) {
					phongMaterial->SetDiffuseTexture(filePathName);
				});
				ImGui::PopID();
				ImGui::NewLine();
				ImGui::PushID(std::to_string(EditorBasic::GetSelectEntity().ID()).c_str());
				if (ImGui::MenuItem("Reset"))phongMaterial->ResetDiffuseTexture();
				ImGui::Columns(1);
				ImGui::PopID();
				ImGui::Separator();

				/*Specular*/
				ImGui::Columns(2, "Specular Texture");
				ImGui::Text("Specular Texture");
				EditorBasic::ShowTexture("Specular preview", phongMaterial->SpecularTexture);

				ImGui::NextColumn();
				ImGui::NewLine();
				if (ImGui::MenuItem("Show Specular"))
					ImGui::OpenPopup("Specular Path");
				if (ImGui::BeginPopup("Specular Path"))
				{
					ImGui::Text((phongMaterial->SpecularTexture->GetPathName()).c_str());
					ImGui::EndPopup();
				}
				ImGui::NewLine();
				ImGui::PushID(std::to_string(EditorBasic::GetSelectEntity().ID()).c_str());
				if (ImGui::MenuItem("Load"))
					EditorBasic::SetPopupFlag("Choose Specular");
				EditorBasic::GetFileDialog("Choose Specular", ".png,.jpg,.bmp,.jpeg", [&](const std::string& filePathName) {
					phongMaterial->SetSpecularTexture(filePathName);
				});
				ImGui::PopID();
				ImGui::PushID(std::to_string(EditorBasic::GetSelectEntity().ID()).c_str());
				ImGui::NewLine();
				if (ImGui::MenuItem("Reset"))phongMaterial->ResetSpecularTexture();
				ImGui::Columns(1);
				ImGui::PopID();
			}
			default:
				break;
			}
		}
	}

	void Inspector::ShowMeshComponent()
	{
		std::unordered_map<MeshType, std::string> KeyMap =
		{
			{MeshType::None, "None"},
			{MeshType::Plane, "Plane"},
			{MeshType::Cube, "Cube"},
			{MeshType::Sphere, "Sphere"},
			{MeshType::Model, "Model"},
		};
		if (ImGui::CollapsingHeader("Mesh"))
		{
			DeleteComponent<MeshComponent>("Mesh");
			
			auto& mesh = EditorBasic::GetSelectEntity().GetComponent<MeshComponent>().Mesh;
			ImGui::Text(("Mesh Type: " + KeyMap[mesh->GetMeshType()]).c_str());
			ImGui::Text(("Path: " + mesh->GetPathName()).c_str());
			ImGui::Text(("Vertices Number: " + std::to_string(mesh->GetVerticesNum())).c_str());
			ImGui::Text(("Triangle Number: " + std::to_string(mesh->GetTriangleNum())).c_str());
			ImGui::Text(("Has TexCoord: " + BoolString(mesh->HasTexCoord())).c_str());
			ImGui::Text(("Has Normal: " + BoolString(mesh->HasNormal())).c_str());
			if (ImGui::Button("Reset")) EditorBasic::GetSelectEntity().GetComponent<MeshComponent>().Reset();
			ImGui::SameLine();
			if (ImGui::Button("Reload"))
				ImGui::OpenPopup("Reload");
			if (ImGui::BeginPopup("Reload"))
			{
				for (auto item : KeyMap)
				{
					if (ImGui::BeginMenu(item.second.c_str()))
					{
						if (item.first != MeshType::None && item.first != MeshType::Model)
						{
							static int sample = 1;
							ImGui::SliderInt("sample", &sample, 1, 30);
							if (ImGui::Button("OK"))
								EditorBasic::GetSelectEntity().GetComponent<MeshComponent>().Reload(item.first, sample);
						}
						if (item.first == MeshType::None)
							if (ImGui::MenuItem("Reset"))
								EditorBasic::GetSelectEntity().GetComponent<MeshComponent>().Reset();
						if (item.first == MeshType::Model)
							if (ImGui::MenuItem("FileBroswer"))
								EditorBasic::SetPopupFlag("Reload Model");
						ImGui::EndMenu();
					}
				}
				ImGui::EndPopup();
			}
			EditorBasic::GetFileDialog("Reload Model", ".obj", [&](const std::string& filePathName) {
				EditorBasic::GetSelectEntity().GetComponent<MeshComponent>().Reload(filePathName);
			});
		}
	}

	void Inspector::ShowLightComponent()
	{
		if (ImGui::CollapsingHeader("Light"))
		{
			DeleteComponent<LightComponent>("Light");

			const char* lightItems[] = { "None", "Basic", "DIrLight", "PointLight", "SpotLight" };
			auto tmp = EditorBasic::GetSelectEntity().GetComponent<LightComponent>().Type;
			auto& light = EditorBasic::GetSelectEntity().GetComponent<LightComponent>();
			ImGui::Combo("Type", (int*)(&light.Type), lightItems, IM_ARRAYSIZE(lightItems));
			if (tmp != light.Type)
				light.ResetType();
			switch (light.Type)
			{
			case LightType::LightType_None:
				break;
			case LightType::LightType_Basic:
				ImGui::ColorEdit3("Color", (float*)(&CastRef<Light>(light.LightSrc)->Color));
				ImGui::DragFloat("Intensity", (float*)(&CastRef<Light>(light.LightSrc)->Intensity), 0.1f);
				break;
			case LightType::LightType_DirLight:
				ImGui::ColorEdit3("Color", (float*)(&CastRef<DirLight>(light.LightSrc)->Color));
				ImGui::DragFloat("Intensity", (float*)(&CastRef<DirLight>(light.LightSrc)->Intensity), 0.1f);
				ImGui::DragFloat3("Direction", (float*)(&CastRef<DirLight>(light.LightSrc)->Direction), 0.1f);
				break;
			case LightType::LightType_PointLight:
				ImGui::ColorEdit3("Color", (float*)(&CastRef<PointLight>(light.LightSrc)->Color));
				ImGui::DragFloat("Intensity", (float*)(&CastRef<PointLight>(light.LightSrc)->Intensity), 0.1f);
				ImGui::DragFloat("constant", (float*)(&CastRef<PointLight>(light.LightSrc)->Constant), 0.01f);
				ImGui::DragFloat("linear", (float*)(&CastRef<PointLight>(light.LightSrc)->Linear), 0.01f);
				ImGui::DragFloat("quadratic", (float*)(&CastRef<PointLight>(light.LightSrc)->Quadratic), 0.01f);
				break;
			case LightType::LightType_SpotLight:
				ImGui::ColorEdit3("Color", (float*)(&CastRef<SpotLight>(light.LightSrc)->Color));
				ImGui::DragFloat("Intensity", (float*)(&CastRef<SpotLight>(light.LightSrc)->Intensity), 0.1f);
				ImGui::DragFloat3("Direction", (float*)(&CastRef<SpotLight>(light.LightSrc)->Direction), 0.1f);
				ImGui::DragFloat("CutOff", (float*)(&CastRef<SpotLight>(light.LightSrc)->CutOff), 0.01f);
				ImGui::DragFloat("OuterCutOff", (float*)(&CastRef<SpotLight>(light.LightSrc)->OuterCutOff), 0.01f);
			default:
				break;
			}
		}
	}


}