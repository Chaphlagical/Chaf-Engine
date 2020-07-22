#include <Editor/menu.h>
#include <imgui.h>
#include <Scene/scene_layer.h>
#include <vector>
#include <string>
#include <iostream>

namespace Chaf
{
    bool Menu::m_AddObjectEvent = false;
    MeshType Menu::m_NewObjectType = MeshType::None;
    int Menu::m_Sample = 1;
    glm::vec3 Menu::m_NewPosition = glm::vec3(0.0f);
    glm::vec3 Menu::m_NewRotation = glm::vec3(0.0f);
    glm::vec3 Menu::m_NewScale = glm::vec3(1.0f);

	void Menu::ShowMainMenu()
	{
        
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Object"))
            {
                if (ImGui::MenuItem("Plane"))
                {
                    m_AddObjectEvent = true;
                    m_NewObjectType = MeshType::Plane;
                }
                if (ImGui::MenuItem("Cube"))
                {
                    m_AddObjectEvent = true;
                    m_NewObjectType = MeshType::Cube;
                }
                if (ImGui::MenuItem("Sphere"))
                {
                    m_AddObjectEvent = true;
                    m_NewObjectType = MeshType::Sphere;
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        AddObject();
	}

    void Menu::AddObject()
    {
        std::vector<std::string> keyMap = { "None", "Plane", "Cube", "Sphere", "Model" };
        if (m_AddObjectEvent && m_NewObjectType != MeshType::None)
        {
            ImGui::Begin(("New "+keyMap[static_cast<int>(m_NewObjectType)]).c_str(), &m_AddObjectEvent);
            ImGui::SliderInt("sample", &m_Sample, 1, 30);
            ImGui::Separator();
            ImGui::DragFloat3("Position", (float*)&m_NewPosition, 0.1f);
            ImGui::DragFloat3("Rotation", (float*)&m_NewRotation, 1.0f);
            ImGui::DragFloat3("Scale", (float*)&m_NewScale, 0.01f);
            bool choose = ImGui::Button("OK");
            if (choose)
            {
                Ref<TriMesh> mesh = CreateRef<TriMesh>(keyMap[static_cast<int>(m_NewObjectType)]);
                mesh->Create(m_NewObjectType, m_Sample);
                mesh->SetPosition(m_NewPosition);
                mesh->SetRotation(m_NewRotation);
                mesh->SetScale(m_NewScale);
                SceneLayer::GetInstance()->PushMesh(mesh);
                m_AddObjectEvent = false;
                m_NewObjectType = MeshType::None;
                Reset();
            }
            ImGui::End();
        }
    }

    void Menu::Reset()
    {
        m_AddObjectEvent = false;
        m_NewObjectType = MeshType::None;
        m_Sample = 1;
        m_NewPosition = glm::vec3(0.0f);
        m_NewRotation = glm::vec3(0.0f);
        m_NewScale = glm::vec3(1.0f);
    }
}