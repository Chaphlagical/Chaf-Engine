#include <Scene/scene_layer.h>
#include <Scene/model.h>
#include <Scene/maincamera_layer.h>
#include <Renderer/command.h>

#include <imgui.h>

namespace Chaf
{
	SceneLayer* SceneLayer::s_Instance = nullptr;

	SceneLayer::SceneLayer()
	{
		s_Instance = this;
		m_DefaultDisplayTexture = Texture2D::Create("assets/texture/checkboard.png");
	}

	void SceneLayer::OnAttach()
	{
		m_Grid = CreateRef<TriMesh>();
		m_Grid->Create(MeshType::Plane, 10);
		m_Grid->SetScale(20.0f, 1.0f, 20.0f);
		m_Grid->SetLineMode(true);

		FrameBufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(fbSpec);
	}

	void SceneLayer::OnUpdate(Timestep timestep)
	{
		MainCameraLayer::GetInstance()->GetCameraController().OnUpdate(timestep);

		m_FrameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		RenderCommand::Clear();
		RenderMesh(MainCameraLayer::GetInstance()->GetCameraController().GetCamera());
		m_FrameBuffer->Unbind();
	}

	void SceneLayer::RenderMesh(Camera& camera)
	{
		DrawDefaultGrid(camera);
		for (auto mesh : m_MeshStack)
			mesh->Draw(camera, m_LineMode);
	}

	void SceneLayer::OnImGuiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Viewport");
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		MainCameraLayer::GetInstance()->GetCameraController().SetActive(ImGui::IsWindowFocused());
		if (m_ViewportSize != *((glm::vec2*) & viewportPanelSize))
		{
			m_ViewportSize = { viewportPanelSize.x,viewportPanelSize.y };
			m_FrameBuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
			MainCameraLayer::GetInstance()->GetCameraController().OnResize(m_ViewportSize.x, m_ViewportSize.y);
		}

		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		static bool showStyleEditor = true;
		ImGui::Begin("Style Editor", &showStyleEditor);
		ImGui::ShowStyleEditor();
		ImGui::End();
	}

	void SceneLayer::OnEvent(Event& event)
	{
		MainCameraLayer::GetInstance()->GetCameraController().OnEvent(event);
	}

}