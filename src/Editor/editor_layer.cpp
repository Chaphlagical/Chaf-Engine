#include <Editor/editor_layer.h>
#include <Scene/scene.h>
#include <Scene/scene_layer.h>
#include <Scene/components.h>
#include <Scene/maincamera_layer.h>
#include <imgui.h>
#include <Editor/FileDialog/ImGuiFileDialog.h>

namespace Chaf
{
	static std::string BoolString(bool x)
	{
		if (x)return "true";
		return "false";
	}

	void EditorLayer::OnAttach()
	{
		m_SelectEntity = SceneLayer::GetInstance()->GetScene()->GetRoot();
		m_WindowHandle = "";
	}

	void EditorLayer::OnImGuiRender()
	{
		if (m_FlagShowHierarchy) ShowHierarchy();
		if (m_FlagShowInspector)	ShowInspector();
		AddObject();
		AddModel();
		ShowMenu();
		if (m_FlagDemoWindow)
			ImGui::ShowDemoWindow(&m_FlagDemoWindow);
		if (m_FlagStyleEditor)
		{
			ImGui::Begin("Style Editor", &m_FlagStyleEditor);
			ImGui::ShowStyleEditor();
			ImGui::End();
		}
	}

	void EditorLayer::ShowHierarchy()
	{
		ImGui::Begin("Hierachy",&m_FlagShowHierarchy);
		auto& scene = SceneLayer::GetInstance()->GetScene();
		if (!scene->Empty())
		{
			auto node = scene->GetRoot();
			while (node)
			{
				ShowHierarchyTree(node);
				node = node.GetNext();
			}
		}
		ImGui::End();
	}

	void EditorLayer::ShowHierarchyTree(Entity& node)
	{
		if (!node)return;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
		bool open = ImGui::TreeNodeEx(std::to_string(node.ID()).c_str(),
			ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen 
			| (node.SameOf(m_SelectEntity) ? ImGuiTreeNodeFlags_Selected : 0) | (node.GetFirstChild() ? 0 : ImGuiTreeNodeFlags_Leaf),
			"%s", node.GetComponent<TagComponent>().Tag.c_str());
		ImGui::PopStyleVar();

		ImGui::PushID(std::to_string(node.ID()).c_str());
		if (ImGui::BeginPopupContextItem())
		{
			ImGui::Text(node.GetComponent<TagComponent>().Tag.c_str());
			MeshType type = MeshType::None;
			//	Add new Object
			if (ImGui::BeginMenu("Create New Object"))
			{
				if (ImGui::MenuItem("Empty"))
				{
					m_WindowHandle = "New Empty Object";
					m_SelectEntity = node;
				}

				ImGui::Separator();
				if (ImGui::MenuItem("Plane"))
				{
					m_WindowHandle = "New Plane";
					m_SelectEntity = node;
				}
				if (ImGui::MenuItem("Cube"))
				{
					m_WindowHandle = "New Cube";
					m_SelectEntity = node;
				}
				if (ImGui::MenuItem("Sphere"))
				{
					m_WindowHandle = "New Sphere";
					m_SelectEntity = node;
				}
				
				ImGui::Separator();
				if (ImGui::MenuItem("Model"))
				{
					m_WindowHandle = "New Model";
					igfd::ImGuiFileDialog::Instance()->OpenDialog("Choose Model", "Choose File", ".obj", ".");
					m_SelectEntity = node;
				}
				ImGui::EndMenu();
			}
			ImGui::Separator();
			// Delete selected object
			if (!node.IsRoot())
			{
				if (ImGui::MenuItem("Delete"))
					RemoveEntity(node);
			}

			//	Rename select object
			if (node)
			{
				ImGui::Separator();
				ImGui::Text("Rename");
				char buf[50];
				auto tmp = node.GetComponent<TagComponent>().Tag;
				for (int i = 0; i < tmp.length(); i++)
					buf[i] = tmp[i];
				buf[tmp.length()] = '\0';
				ImGui::InputText("##edit", buf, 50);
				node.GetComponent<TagComponent>().Tag = buf;
				if (ImGui::Button("OK"))
					ImGui::CloseCurrentPopup();
			}
			
			ImGui::EndPopup();
		}
		
		ImGui::PopID();

		if (ImGui::IsItemClicked()) 
		{
			m_SelectEntity = node;
		}

		if (ImGui::BeginDragDropSource()) 
		{
			if (!m_SelectEntity.SameOf(SceneLayer::GetInstance()->GetScene()->GetRoot()))
			{
				ImGui::SetDragDropPayload("Hierarchy Drag&Drop", &m_SelectEntity, sizeof(Entity));
				ImGui::Text(node.GetComponent<TagComponent>().Tag.c_str());
			}
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget()) 
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Hierarchy Drag&Drop"))
			{
				CHAF_CORE_ASSERT(payload->DataSize == sizeof(Entity), "payload error!");
				Entity srcNode = *(const Entity*)payload->Data;
				srcNode.MoveAsChildOf(node);
				auto& transform = srcNode.GetComponent<TransformComponent>();
				transform.SetRelatePosition(srcNode.GetParent().GetComponent<TransformComponent>().Position);
			}
			ImGui::EndDragDropTarget();
		}
		if (open)
		{
			if (node)
			{
				auto child = node.GetFirstChild();
				
				while (child)
				{
					ShowHierarchyTree(child);
					child = child.GetNext();
				}
			}
			ImGui::TreePop();
		}
	}

	//	Spector
	void EditorLayer::ShowInspector()
	{
		ImGui::Begin("Inspector", &m_FlagShowInspector);
		if (m_SelectEntity && m_SelectEntity.HasComponent<TagComponent>() && m_SelectEntity.GetComponent<TagComponent>().Tag != "scene")
		{
			std::vector<std::string> addComponentItem;
			ShowTransformComponent();
			if (m_SelectEntity.HasComponent<MeshComponent>())
				ShowMeshComponent();
			else addComponentItem.push_back("Mesh Component");

			if (m_SelectEntity.HasComponent<MaterialComponent>())
				ShowMaterialComponent();
			else addComponentItem.push_back("Material Component");
			
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
						AddComponentMapping(component);
				ImGui::EndPopup();
			}
		}	
		ImGui::End();
	}

	void EditorLayer::ShowMenu()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				bool linemode = SceneLayer::GetInstance()->GetScene()->GetLineMode();
				std::string display = "";
				if (linemode)
					display = "set polygon";
				else display = "set wireframe";
				if (ImGui::MenuItem(display.c_str())) linemode = !linemode;
				SceneLayer::GetInstance()->GetScene()->SetLineMode(linemode);
				bool gridFlag = SceneLayer::GetInstance()->IsShowGrid();
				ImGui::MenuItem("Grid", NULL, &gridFlag);
				SceneLayer::GetInstance()->SetShowGrid(gridFlag);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Obejct"))
			{
				MeshType type = MeshType::None;
				if (ImGui::BeginMenu("New 3D Object"))
				{
					//	Add new Object
					if (ImGui::MenuItem("Empty"))
						m_WindowHandle = "New Empty Object";

					ImGui::Separator();
					if (ImGui::MenuItem("Plane"))
						m_WindowHandle = "New Plane";
					if (ImGui::MenuItem("Cube"))
						m_WindowHandle = "New Cube";
					if (ImGui::MenuItem("Sphere"))
						m_WindowHandle = "New Sphere";

					ImGui::Separator();
					if (ImGui::MenuItem("Model"))
					{
						m_WindowHandle = "New Model";
						igfd::ImGuiFileDialog::Instance()->OpenDialog("Choose Model", "Choose File", ".obj", ".");
					}
					ImGui::EndMenu();
				}
				//	TODO: Light/Camera
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Component"))
			{
				if (ImGui::MenuItem("Material Component"))
					AddComponentMapping("Material Component");
				if (ImGui::MenuItem("Mesh Component"))
					AddComponentMapping("Mesh Component");
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Window"))
			{
				ImGui::MenuItem("Hierachy", NULL, &m_FlagShowHierarchy);
				ImGui::MenuItem("Inspector", NULL, &m_FlagShowInspector);
				ImGui::MenuItem("DemoWindow", NULL, &m_FlagDemoWindow);
				ImGui::MenuItem("StyleEditor", NULL, &m_FlagStyleEditor);
				ImGui::MenuItem("Camera Setting", NULL, &MainCameraLayer::GetInstance()->GetWindowHandle());
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void EditorLayer::ShowTransformComponent()
	{
		ImGui::Text(m_SelectEntity.GetComponent<TagComponent>().Tag.c_str());
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::DragFloat3("Position", (float*)&m_SelectEntity.GetComponent<TransformComponent>().Position, 0.1f);
			ImGui::DragFloat3("Rotation", (float*)&m_SelectEntity.GetComponent<TransformComponent>().Rotation, 1.0f);
			ImGui::DragFloat3("Scale", (float*)&m_SelectEntity.GetComponent<TransformComponent>().Scale, 0.01f);
			ImGui::Separator();
		}
	}

	void EditorLayer::ShowMaterialComponent()
	{
		if (ImGui::CollapsingHeader("Material"))
		{
			if (ImGui::BeginPopupContextItem("delete"))
			{
				if (ImGui::MenuItem("Delete Component"))
				{
					m_SelectEntity.RemoveComponent<MaterialComponent>();
					ImGui::End();
					return;
				}
				ImGui::EndPopup();
			}

			ImGui::Text("Albedo Color");
			ImGui::ColorEdit4("color", (float*)&(m_SelectEntity.GetComponent<MaterialComponent>().Color));
			ImGui::Separator();

			ImGui::Columns(2, "Albedo Texture");
			ImGui::Text("Albedo Texture");
			auto textureID = m_SelectEntity.GetComponent<MaterialComponent>().HasAlbedo ?
				m_SelectEntity.GetComponent<MaterialComponent>().AlbedoTexture->GetRendererID() :
				SceneLayer::GetDefaultRenderData()->checkboardTexture->GetRendererID();
			ImGui::Image((void*)textureID, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
			if (ImGui::IsItemHovered()&&ImGui::IsMouseReleased(ImGuiMouseButton_Left))
				ImGui::OpenPopup("image preview");
			if (ImGui::BeginPopup("image preview"))
			{
				ImGui::Image((void*)textureID, ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
				ImGui::EndPopup();
			}
			ImGui::NextColumn();
			ImGui::NewLine();
			if (ImGui::MenuItem("Show Path"))
				ImGui::OpenPopup("Texture Path");
			if (ImGui::BeginPopup("Texture Path"))
			{
				ImGui::Text((m_SelectEntity.GetComponent<MaterialComponent>().AlbedoTexture->GetPathName()).c_str());
				ImGui::EndPopup();
			}
			ImGui::NewLine();
			ImGui::PushID(std::to_string(m_SelectEntity.ID()).c_str());
			if (ImGui::MenuItem("Load"))
			{
				igfd::ImGuiFileDialog::Instance()->OpenDialog("Choose Texture", "Choose File", ".png,.jpg,.bmp,.jpeg,.hdr", ".");
			}
			if (igfd::ImGuiFileDialog::Instance()->FileDialog("Choose Texture"))
			{
				if (igfd::ImGuiFileDialog::Instance()->IsOk == true)
				{
					std::string filePathName = igfd::ImGuiFileDialog::Instance()->GetFilepathName();
					m_SelectEntity.GetComponent<MaterialComponent>().SetTexture(filePathName);
				}
				igfd::ImGuiFileDialog::Instance()->CloseDialog("Choose Texture");
			}
			ImGui::PopID();
			ImGui::PushID(std::to_string(m_SelectEntity.ID()).c_str());
			ImGui::NewLine();
			if (ImGui::MenuItem("Reset"))m_SelectEntity.GetComponent<MaterialComponent>().ResetTexture();
			ImGui::Columns(1);
			ImGui::PopID();
		}
	}

	void EditorLayer::ShowMeshComponent()
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
			if (ImGui::BeginPopupContextItem("delete"))
			{
				if (ImGui::MenuItem("Delete Component"))
				{
					m_SelectEntity.RemoveComponent<MeshComponent>();
					ImGui::End();
					return;
				}
				ImGui::EndPopup();
			}

			auto& mesh = m_SelectEntity.GetComponent<MeshComponent>().Mesh;
			ImGui::Text(("Mesh Type: " + KeyMap[mesh->GetMeshType()]).c_str());
			ImGui::Text(("Path: " + mesh->GetPathName()).c_str());
			ImGui::Text(("Vertices Number: " + std::to_string(mesh->GetVerticesNum())).c_str());
			ImGui::Text(("Triangle Number: " + std::to_string(mesh->GetTriangleNum())).c_str());
			ImGui::Text(("Has TexCoord: " + BoolString(mesh->HasTexCoord())).c_str());
			ImGui::Text(("Has Normal: " + BoolString(mesh->HasNormal())).c_str());
			if (ImGui::Button("Reset")) m_SelectEntity.GetComponent<MeshComponent>().Reset();
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
								m_SelectEntity.GetComponent<MeshComponent>().Reload(item.first, sample);
						}
						if (item.first == MeshType::None)
							if(ImGui::MenuItem("Reset"))
								m_SelectEntity.GetComponent<MeshComponent>().Reset();
						if (item.first == MeshType::Model)
							if(ImGui::MenuItem("FileBroswer"))
								igfd::ImGuiFileDialog::Instance()->OpenDialog("Reload Model", "Choose File", ".obj", ".");
						ImGui::EndMenu();
					}
				}
				ImGui::EndPopup();
			}
			if (igfd::ImGuiFileDialog::Instance()->FileDialog("Reload Model"))
			{
				if (igfd::ImGuiFileDialog::Instance()->IsOk == true)
				{
					std::string filePathName = igfd::ImGuiFileDialog::Instance()->GetFilepathName();
					m_SelectEntity.GetComponent<MeshComponent>().Reload(filePathName);
				}
				igfd::ImGuiFileDialog::Instance()->CloseDialog("Reload Model");
			}
		}
	}

	void EditorLayer::RemoveEntity(Entity& node)
	{
		auto tmp = node;
		node = node.GetNext();
		tmp.Remove();
		ImGui::CloseCurrentPopup();
	}

	void EditorLayer::AddObject()
	{
		std::unordered_map<std::string, MeshType> KeyMap =
		{
			{"New Empty Object", MeshType::None},
			{"New Plane", MeshType::Plane},
			{"New Cube", MeshType::Cube},
			{"New Sphere", MeshType::Sphere},
		};

		if (KeyMap.count(m_WindowHandle) == 0)return;

		auto key = KeyMap[m_WindowHandle];

		ImGui::Begin(m_WindowHandle.c_str());

		ImGui::Text("Transform");
		static glm::vec3 position{ 0.0f };
		static glm::vec3 rotation{ 0.0f };
		static glm::vec3 scale{ 1.0f };
		ImGui::DragFloat3("Position", (float*)&position, 0.1f);
		ImGui::DragFloat3("Rotation", (float*)&rotation, 1.0f);
		ImGui::DragFloat3("Scale", (float*)&scale, 0.01f);

		static int sample = 1;
		if (key != MeshType::None)
		{
			ImGui::Separator();
			ImGui::Text("Sample");
			ImGui::SliderInt("sample", &sample, 1, 30);
		}
		ImGui::Separator();
		ImGui::Text("Name");
		static char buf[32] = "New Object";
		ImGui::InputText("##edit", buf, 50);

		ImGui::Separator();

		if (ImGui::Button("Create"))
		{
			auto entity = m_SelectEntity.CreateChild(std::string(buf));
			if (key != MeshType::None)
				entity.AddComponent<MeshComponent>(key, sample);
			entity.GetComponent<TransformComponent>().Position = position;
			entity.GetComponent<TransformComponent>().Rotation = rotation;
			entity.GetComponent<TransformComponent>().Scale = scale;
			entity.GetComponent<TransformComponent>().Update();
			m_WindowHandle = "";
			m_SelectEntity = entity;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
			m_WindowHandle = "";
		}
		ImGui::End();		
	}

	void EditorLayer::AddModel()
	{
		if (m_WindowHandle != "New Model")return;
		if (igfd::ImGuiFileDialog::Instance()->FileDialog("Choose Model"))
		{
			if (igfd::ImGuiFileDialog::Instance()->IsOk == true)
			{
				std::string path = igfd::ImGuiFileDialog::Instance()->GetFilepathName();
				auto entity = m_SelectEntity.CreateChild(std::string("mesh"));
				entity.AddComponent<MeshComponent>(path);
				m_WindowHandle = "";
				m_SelectEntity = entity;
			}
			igfd::ImGuiFileDialog::Instance()->CloseDialog("Choose Model");
		}
	}

	void EditorLayer::AddComponentMapping(std::string key)
	{
		if (key == "Mesh Component" && !m_SelectEntity.HasComponent<MeshComponent>())
			m_SelectEntity.AddComponent<MeshComponent>();
		else if (key == "Material Component" && !m_SelectEntity.HasComponent<MaterialComponent>())
			m_SelectEntity.AddComponent<MaterialComponent>();
	}
}