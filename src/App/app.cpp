#include "app.h"
#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Chaf
{
    EditorLayer::EditorLayer()
        : Layer("EditorLayer")
    {

    }

    void EditorLayer::OnAttach()
    {
    }

    void EditorLayer::OnDetach()
    {
        
    }

    void EditorLayer::OnUpdate(Timestep timestep)
    {
    }

    void EditorLayer::OnImGuiRender()
    {
       

        ImGui::Begin("Settings");
        ImGui::Text("Hello World");
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
        ImGui::Begin("Viewport");
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        //Application::Get().GetImGuiLayer()->SetBlockEvents(!ImGui::IsWindowFocused());
        ImGui::End();
        ImGui::PopStyleVar();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

       

    }

    void EditorLayer::OnEvent(Event& event)
    {
    }
}