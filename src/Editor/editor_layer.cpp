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

namespace Chaf
{
	void EditorLayer::OnAttach()
	{
	}

	void EditorLayer::OnImGuiRender()
	{
		if (EditorBasic::m_FlagShowHierarchy) Hierachy::ShowHierarchy(&EditorBasic::m_FlagShowHierarchy);
		if (EditorBasic::m_FlagShowInspector) Inspector::ShowInspector(&EditorBasic::m_FlagShowInspector);
		Menu::ShowMainMenu();
		EditorBasic::AddObjectAnswer();
	}
}