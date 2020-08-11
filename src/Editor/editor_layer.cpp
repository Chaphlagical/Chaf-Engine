#include <Editor/editor_layer.h>
#include <Scene/scene.h>
#include <Scene/scene_layer.h>
#include <Scene/components.h>
#include <Scene/maincamera_layer.h>
#include <imgui.h>
#include <Editor/FileDialog/ImGuiFileDialog.h>
#include <Editor/hierarchy.h>
#include <Editor/inspector.h>
#include <Editor/menu.h>
#include <Editor/basic.h>
#include <Editor/terminal.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace Chaf
{
	void EditorLayer::OnAttach()
	{
		SceneLayer::SetGuizmoFunc([&](glm::vec2 viewportSize) {
			ImVec2 size = ImGui::GetContentRegionAvail();
			ImVec2 cursorPos = ImGui::GetCursorScreenPos();          // "cursor" is where imgui will draw the image
			ImGuizmo::SetRect(cursorPos.x, cursorPos.y, size.x, size.y);
			if (EditorBasic::GetSelectEntity()&&!EditorBasic::GetSelectEntity().IsRoot())
			{
				ImGuizmo::Manipulate(glm::value_ptr(MainCameraLayer::GetInstance()->GetCameraController().GetCamera().GetViewMatrix()),
					glm::value_ptr(MainCameraLayer::GetInstance()->GetCameraController().GetCamera().GetProjectionMatrix()),
					EditorBasic::mCurrentGizmoOperation, EditorBasic::mCurrentGizmoMode,
					&EditorBasic::GetSelectEntity().GetComponent<TransformComponent>().Transform[0][0]
				);
				ImGuizmo::DecomposeMatrixToComponents(&EditorBasic::GetSelectEntity().GetComponent<TransformComponent>().Transform[0][0], 
					&EditorBasic::GetSelectEntity().GetComponent<TransformComponent>().Position[0], 
					&EditorBasic::GetSelectEntity().GetComponent<TransformComponent>().Rotation[0], 
					&EditorBasic::GetSelectEntity().GetComponent<TransformComponent>().Scale[0]);
				EditorBasic::GetSelectEntity().GetComponent<TransformComponent>().Position = EditorBasic::GetSelectEntity().GetComponent<TransformComponent>().Position - EditorBasic::GetSelectEntity().GetComponent<TransformComponent>().RelatePosition;
			}
		});
	}

	void EditorLayer::OnImGuiRender()
	{
		if (EditorBasic::m_FlagShowHierarchy) Hierachy::ShowHierarchy(&EditorBasic::m_FlagShowHierarchy);
		if (EditorBasic::m_FlagShowInspector) Inspector::ShowInspector(&EditorBasic::m_FlagShowInspector);
		if (EditorBasic::m_FlagTerminal) Terminal::LoggingConsole(&EditorBasic::m_FlagTerminal);
		if (EditorBasic::m_FlagDemoWindow)ImGui::ShowDemoWindow(&EditorBasic::m_FlagDemoWindow);

		if (EditorBasic::m_FlagStyleEditor)
		{
			ImGui::Begin("Style Editor", &EditorBasic::m_FlagStyleEditor);
			ImGui::ShowStyleEditor();
			ImGui::End();
		}
		Menu::ShowMainMenu();
		Menu::ShowGuizmoMenu();
		EditorBasic::AddObjectAnswer();
	}
}