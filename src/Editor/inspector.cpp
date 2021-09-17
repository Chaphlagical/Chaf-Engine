#include <Editor/inspector.h>
#include <Editor/FileDialog/ImFileDialog.h>
#include <Editor/menu.h>

namespace Chaf
{
	void Inspector::ShowInspector(bool* p_open)
	{
		ImGui::Begin("Inspector", p_open, ImGuiWindowFlags_MenuBar);
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
			glm::vec3& position = EditorBasic::GetSelectEntity().GetComponent<TransformComponent>().Position;
			glm::vec3& rotation= EditorBasic::GetSelectEntity().GetComponent<TransformComponent>().Rotation;
			glm::vec3& scale = EditorBasic::GetSelectEntity().GetComponent<TransformComponent>().Scale;
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
			if (DeleteComponent<MaterialComponent>("Material"))return;

			//	
			const char* materialItems[] = { "None", "Emisson", "Phong", "Cook Torrance BRDF" };
			auto tmp = EditorBasic::GetSelectEntity().GetComponent<MaterialComponent>().Type;
			auto& material = EditorBasic::GetSelectEntity().GetComponent<MaterialComponent>();
			ImGui::PushID(EditorBasic::GetSelectEntity().ID());
			ImGui::Combo("Material Type", (int*)(&material.Type), materialItems, IM_ARRAYSIZE(materialItems));
			ImGui::PopID();
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

				auto& phongMaterial = CastRef<PhongMaterial>(EditorBasic::GetSelectEntity().GetComponent<MaterialComponent>().MaterialSrc);

				ShowSetTexture<Ref<PhongMaterial>>(phongMaterial->DiffuseTexture, "Diffuse", phongMaterial);
				ImGui::Separator();
				ShowSetTexture<Ref<PhongMaterial>>(phongMaterial->SpecularTexture, "Specular", phongMaterial);
				ImGui::Separator();
				ShowSetTexture<Ref<PhongMaterial>>(phongMaterial->NormalTexture, "Normal", phongMaterial);
				ImGui::Separator();
				ShowSetTexture<Ref<PhongMaterial>>(phongMaterial->DisplacementTexture, "Displacement", phongMaterial);
				ImGui::DragFloat("Height Scale", &phongMaterial->HeightScale, 0.01f);
				break;
			}
			case MaterialType::Material_Cook_Torrance:
			{
				ImGui::ColorEdit3("Color", (float*)(&CastRef<CookTorranceBRDF>(material.MaterialSrc)->Color));
				ImGui::Separator();

				auto& CookTorranceMaterial = CastRef<CookTorranceBRDF>(EditorBasic::GetSelectEntity().GetComponent<MaterialComponent>().MaterialSrc);

				ShowSetTexture<Ref<CookTorranceBRDF>>(CookTorranceMaterial->AlbedoTexture, "Albedo", CookTorranceMaterial);
				ImGui::Separator();
				ShowSetTexture<Ref<CookTorranceBRDF>>(CookTorranceMaterial->NormalTexture, "Normal", CookTorranceMaterial);
				
				ImGui::Separator();
				ShowSetTexture<Ref<CookTorranceBRDF>>(CookTorranceMaterial->MetallicTexture, "Metallic", CookTorranceMaterial);
				ImGui::SliderFloat("Metallic", &CookTorranceMaterial->Metallic, 0.0f, 1.0f, "%.2f");
				ImGui::Separator();
				ShowSetTexture<Ref<CookTorranceBRDF>>(CookTorranceMaterial->RoughnessTexture, "Roughness", CookTorranceMaterial);
				ImGui::SliderFloat("Roughness", &CookTorranceMaterial->Roughness, 0.0f, 1.0f, "%.2f");
				ShowSetTexture<Ref<CookTorranceBRDF>>(CookTorranceMaterial->AOTexture, "AO", CookTorranceMaterial);
				ShowSetTexture<Ref<CookTorranceBRDF>>(CookTorranceMaterial->DisplacementTexture, "Displacement", CookTorranceMaterial);
				ImGui::DragFloat("Height Scale", &CookTorranceMaterial->HeightScale, 0.01f);
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
			if (DeleteComponent<MeshComponent>("Mesh"))return;
			
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
			if (DeleteComponent<LightComponent>("Light"))return;

			const char* lightItems[] = { "None", "Basic", "DIrLight", "PointLight", "SpotLight" };
			auto tmp = EditorBasic::GetSelectEntity().GetComponent<LightComponent>().Type;
			auto& light = EditorBasic::GetSelectEntity().GetComponent<LightComponent>();
			ImGui::PushID(EditorBasic::GetSelectEntity().ID());
			ImGui::Combo("Light Type", (int*)(&light.Type), lightItems, IM_ARRAYSIZE(lightItems));
			ImGui::PopID();
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
				ImGui::DragFloat("Intensity", (float*)(&CastRef<DirLight>(light.LightSrc)->Intensity), 0.1f, 0.0f);
				ImGui::DragFloat3("Direction", (float*)(&CastRef<DirLight>(light.LightSrc)->Direction), 0.1f);
				break;
			case LightType::LightType_PointLight:
				ImGui::ColorEdit3("Color", (float*)(&CastRef<PointLight>(light.LightSrc)->Color));
				ImGui::DragFloat("Intensity", (float*)(&CastRef<PointLight>(light.LightSrc)->Intensity), 0.1f, 0.0f);
				ImGui::DragFloat("constant", (float*)(&CastRef<PointLight>(light.LightSrc)->Constant), 0.01f, 0.0f);
				ImGui::DragFloat("linear", (float*)(&CastRef<PointLight>(light.LightSrc)->Linear), 0.01f, 0.0f);
				ImGui::DragFloat("quadratic", (float*)(&CastRef<PointLight>(light.LightSrc)->Quadratic), 0.01f, 0.0f);
				break;
			case LightType::LightType_SpotLight:
				ImGui::ColorEdit3("Color", (float*)(&CastRef<SpotLight>(light.LightSrc)->Color));
				ImGui::DragFloat("Intensity", (float*)(&CastRef<SpotLight>(light.LightSrc)->Intensity), 0.1f, 0.0f);
				ImGui::DragFloat3("Direction", (float*)(&CastRef<SpotLight>(light.LightSrc)->Direction), 0.1f, 0.0f);
				ImGui::DragFloat("CutOff", (float*)(&CastRef<SpotLight>(light.LightSrc)->CutOff), 0.01f, 0.0f);
				ImGui::DragFloat("OuterCutOff", (float*)(&CastRef<SpotLight>(light.LightSrc)->OuterCutOff), 0.01f, 0.0f);
			default:
				break;
			}
		}
	}
}