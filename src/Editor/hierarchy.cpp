#include <Editor/hierarchy.h>
#include <Scene/scene_layer.h>
#include <Scene/components.h>
#include <Editor/basic.h>
#include <imgui.h>

namespace Chaf
{
	void Hierachy::ShowHierarchy(bool* p_open)
	{
		ImGui::Begin("Hierachy", p_open);
		auto& scene = SceneLayer::GetInstance()->GetScene();
		if (!scene->Empty())
		{
			auto node = scene->GetRoot();
			while (node)
			{
				RecurseTree(node);
				node = node.GetNext();
			}
		}
		ImGui::End();
	}

	void Hierachy::RecurseTree(Entity node)
	{
		if (!node)return;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
		bool open = ImGui::TreeNodeEx(std::to_string(node.ID()).c_str(),
			ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen
			| (node.SameOf(EditorBasic::GetSelectEntity()) ? ImGuiTreeNodeFlags_Selected : 0) | (node.GetFirstChild() ? 0 : ImGuiTreeNodeFlags_Leaf),
			"%s", node.GetComponent<TagComponent>().Tag.c_str());
		ImGui::PopStyleVar();

		ImGui::PushID(std::to_string(node.ID()).c_str());
		if (ImGui::BeginPopupContextItem())
		{
			ImGui::Text(node.GetComponent<TagComponent>().Tag.c_str());
			//	new object menu
			EditorBasic::AddObjectMenu(node);
			ImGui::Separator();
			//	delete
			if (!node.IsRoot())
			{
				if (ImGui::MenuItem("Delete"))
					RemoveEntity(node);
			}
			//	rename
			if (node)
			{
				ImGui::Separator();
				EditorBasic::Rename(node);
			}
			ImGui::EndPopup();
		}
		ImGui::PopID();

		//	left click check
		if (ImGui::IsItemClicked()) EditorBasic::SetSelectEntity(node);
		//	Drag&Drop Effect
		EditorBasic::SetDragDropSource<Entity>("Hierarchy Drag&Drop", [&](){
			ImGui::Text(node.GetComponent<TagComponent>().Tag.c_str());
		}, EditorBasic::GetSelectEntity());

		EditorBasic::SetDragDropTarget<Entity>("Hierarchy Drag&Drop", [&](Entity srcNode) {
			srcNode.MoveAsChildOf(node);
			auto& transform = srcNode.GetComponent<TransformComponent>();
			transform.SetRelatePosition(srcNode.GetParent().GetComponent<TransformComponent>().Position);
		});
		//	Recurse
		if (open)
		{
			if (node)
			{
				auto child = node.GetFirstChild();
				while (child)
				{
					RecurseTree(child);
					child = child.GetNext();
				}
			}
			ImGui::TreePop();
		}
	}

	void Hierachy::RemoveEntity(Entity& node)
	{
		auto tmp = node;
		node = node.GetNext();
		tmp.Remove();
		ImGui::CloseCurrentPopup();
	}
}