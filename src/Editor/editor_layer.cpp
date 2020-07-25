#include <Editor/editor_layer.h>
#include <Scene/scene.h>
#include <Scene/scene_layer.h>
#include <Scene/components.h>
#include <imgui.h>
#include <unordered_map>
#include <Editor/FileDialog/ImGuiFileDialog.h>

namespace Chaf
{
	void EditorLayer::OnAttach()
	{
		m_SelectEntity = Entity();
		m_WindowHandle = "";
	}

	void EditorLayer::OnImGuiRender()
	{
		ShowHierarchy();
		ShowInspector();
		AddObject();
		AddModel();
	}

	void EditorLayer::ShowHierarchy()
	{
		ImGui::Begin("Hierachy");
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
			if (ImGui::BeginMenu("New"))
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

	void EditorLayer::ShowInspector()
	{
		
		ImGui::Begin("Inspector");
		if (m_SelectEntity && m_SelectEntity.HasComponent<TagComponent>() && m_SelectEntity.GetComponent<TagComponent>().Tag != "scene")
		{
			ShowTransformComponent();
			if (m_SelectEntity.HasComponent<MaterialComponent>())
				ShowMaterialComponent();
		}	

		ImGui::End();
	}

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
			ImGui::Text("Albedo Color");
			ImGui::ColorEdit4("color", (float*)&(m_SelectEntity.GetComponent<MaterialComponent>().Color));
			ImGui::Separator();

			ImGui::Columns(2, "Albedo Texture");
			auto textureID = m_SelectEntity.GetComponent<MaterialComponent>().HasAlbedo ?
				m_SelectEntity.GetComponent<MaterialComponent>().AlbedoTexture->GetRendererID() :
				SceneLayer::GetDefaultRenderData()->whiteTexture->GetRendererID();
			ImGui::Image((void*)textureID, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
			/*ImGui::PushID(std::to_string(m_SelectEntity.ID()).c_str());
			if (ImGui::BeginPopupContextItem()) 
			{
				ImGui::Image((void*)textureID, ImVec2(640, 640), ImVec2(0, 1), ImVec2(1, 0));
				ImGui::EndPopup();
			}
			ImGui::PopID();*/

		}
	}

	void EditorLayer::RemoveEntity(Entity& node)
	{
		auto tmp = node;
		node = node.GetNext();
		tmp.Remove();
		ImGui::CloseCurrentPopup();
	}

	void EditorLayer::AddEntity(Entity& node)
	{
		if (ImGui::MenuItem("Plane"))
		{
			SceneLayer::GetInstance()->GetScene()->CreateEntity();
			
		}
	}

	void EditorLayer::AddObject()
	{
		std::vector<std::string> keyMap = { "New Empty Object", "New Plane", "New Cube", "New Sphere" };

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
}