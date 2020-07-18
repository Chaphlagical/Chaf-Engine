#include "app.h"
#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Renderer/command.h>
#include <Renderer/camera.h>
#include <Renderer/Renderer2D.h>

namespace Chaf
{
    EditorLayer::EditorLayer()
        : Layer("EditorLayer"), m_CameraController(23.0f, 16.0f / 9.0f, CameraType::Perspective)
    {
        Renderer2D::Init();
    }

    void EditorLayer::OnAttach()
    {
        m_Texture = Texture2D::Create("assets/texture/test.png");

        FrameBufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_FrameBuffer = FrameBuffer::Create(fbSpec);
    }

    void EditorLayer::OnDetach()
    {
        
    }

    void EditorLayer::OnUpdate(Timestep timestep)
    {
        //	Update
        m_CameraController.OnUpdate(timestep);

        //	Render

        m_FrameBuffer->Bind();
        RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
        RenderCommand::Clear();
        Renderer2D::BeginScene(m_CameraController.GetCamera());
        //Renderer2D::SetLineMode(true);
        Renderer2D::DrawQuad({ -1.0f,0.0f }, { 0.8f,0.8f }, { 0.8f,0.2f,0.3f,1.0f });
        Renderer2D::DrawQuad({ 0.5f,-0.5f }, { 0.5f,0.75f }, { 0.2f,0.3f,0.8f,1.0f });
        Renderer2D::DrawQuad({ 0.0f,0.0f,-0.1f }, { 10.0f,10.0f }, m_Texture, { 0.2f,0.8f,0.3f,1.0f });
        Renderer2D::EndScene();
        m_FrameBuffer->Unbind();
    }

    void EditorLayer::OnImGuiRender()
    {
       

        ImGui::Begin("Settings");
        ImGui::ColorEdit3("color", glm::value_ptr(m_SquareColor));
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
        ImGui::Begin("Viewport");
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        //Application::Get().GetImGuiLayer()->SetBlockEvents(!ImGui::IsWindowFocused());
        m_CameraController.SetActive(ImGui::IsWindowFocused());
        if (m_ViewportSize != *((glm::vec2*) & viewportPanelSize))
        {
            m_ViewportSize = { viewportPanelSize.x,viewportPanelSize.y };
            m_FrameBuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);

            m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
        }

        uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
        ImGui::PopStyleVar();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);
       

    }

    void EditorLayer::OnEvent(Event& event)
    {
        m_CameraController.OnEvent(event);
    }
}