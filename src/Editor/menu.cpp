#include <Editor/menu.h>
#include <Scene/maincamera_layer.h>
#include <Scene/scene_layer.h>
#include <Scene/maincamera_layer.h>
#include <Editor/basic.h>
#include <Editor/FileDialog/ImGuiFileDialog.h>
#include <Gui/Guizmo/ImGuizmo.h>
#include <imgui.h>
#include <string>
#include <fstream>
#include <iostream>

namespace Chaf
{
	void Menu::ShowMainMenu()
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
			if (ImGui::BeginMenu("Environment"))
			{
				//Load Skybox
				if (ImGui::BeginMenu("Skybox"))
				{
					ImGui::Image((void*)SceneLayer::GetInstance()->GetSkybox()->GetTextureID(), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
					if (ImGui::BeginPopupContextItem("Delete Skybox"))
					{
						if (ImGui::MenuItem("Delete"))
						{
							SceneLayer::GetInstance()->GetSkybox().reset();
							SceneLayer::GetInstance()->GetSkybox() = Cubemap::Create();
						}
						ImGui::EndPopup();
					}
					if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
						EditorBasic::SetPopupFlag("Choose Skybox");
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			EditorBasic::GetFileDialog("Choose Skybox", ".hdr", [&](const std::string& filePathName) {
				SceneLayer::GetInstance()->GetSkybox().reset();
				SceneLayer::GetInstance()->GetSkybox() = Cubemap::Create(filePathName);
			});
			/*Add Object*/
			if (ImGui::BeginMenu("Obejct"))
			{
				MeshType type = MeshType::None;
				EditorBasic::AddObjectMenu(EditorBasic::GetSelectEntity());
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Component"))
			{
				if (ImGui::MenuItem("Material Component"))
					EditorBasic::AddComponent<MaterialComponent>();
				if (ImGui::MenuItem("Mesh Component"))
					EditorBasic::AddComponent<MeshComponent>();
				if (ImGui::MenuItem("Light Component"))
					EditorBasic::AddComponent<LightComponent>();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Window"))
			{
				ImGui::MenuItem("Hierachy", NULL, &EditorBasic::m_FlagShowHierarchy);
				ImGui::MenuItem("Inspector", NULL, &EditorBasic::m_FlagShowInspector);
				ImGui::MenuItem("DemoWindow", NULL, &EditorBasic::m_FlagDemoWindow);
				ImGui::MenuItem("StyleEditor", NULL, &EditorBasic::m_FlagStyleEditor);
				ImGui::MenuItem("Terminal", NULL, &EditorBasic::m_FlagTerminal);
				ImGui::MenuItem("Camera Setting", NULL, &MainCameraLayer::GetInstance()->GetWindowHandle());
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("About"))
			{
				ShowInformation();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}

	void Menu::ShowGuizmoMenu()
	{
		if (EditorBasic::m_FlagShowInspector)
		{
			ImGui::Begin("Inspector", &EditorBasic::m_FlagShowInspector);
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::RadioButton("Translate", EditorBasic::mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
				{
					EditorBasic::mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
					EditorBasic::mCurrentGizmoMode = ImGuizmo::WORLD;
				}

				if (ImGui::RadioButton("Rotate", EditorBasic::mCurrentGizmoOperation == ImGuizmo::ROTATE))
				{
					EditorBasic::mCurrentGizmoOperation = ImGuizmo::ROTATE;
					EditorBasic::mCurrentGizmoMode = ImGuizmo::LOCAL;
				}

				if (ImGui::RadioButton("Scale", EditorBasic::mCurrentGizmoOperation == ImGuizmo::SCALE))
					EditorBasic::mCurrentGizmoOperation = ImGuizmo::SCALE;

				ImGui::EndMenuBar();
			}
			ImGui::End();
		}
	}

	void Menu::ShowInformation()
	{
		std::fstream file;
		file.open("log.txt");

		std::string s;
		for (int i = 0; i < 4; i++)
		{
			std::getline(file, s);
			ImGui::Text(s.substr(46).data());
		}
		file.close();
	}
}