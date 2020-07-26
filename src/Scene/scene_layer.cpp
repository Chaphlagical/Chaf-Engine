#include <Scene/scene_layer.h>
#include <Scene/components.h>
#include <Scene/entity.h>
#include <Scene/maincamera_layer.h>
#include <imgui.h>
#include <Renderer/command.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Chaf
{
	SceneLayer* SceneLayer::s_Instance = nullptr;
	Ref<SceneRenderData> SceneLayer::m_DefaultSceneRenderData = nullptr;

	SceneLayer::SceneLayer()
	{
		s_Instance = this;
		m_MainScene = CreateRef<Scene>();
		m_DefaultSceneRenderData = CreateRef<SceneRenderData>();
	}

	void SceneLayer::OnAttach()
	{
		m_Grid = CreateRef<TriMesh>();
		m_Grid->Create(MeshType::Plane, 10);

		FrameBufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(fbSpec);

		m_Shader = Shader::Create("assets/shader/texture.glsl");
		m_Shader->SetInt("u_Texture", 0);
	}

	void SceneLayer::DrawGrid()
	{
		m_Shader->SetMat4("u_Transform", glm::scale(m_DefaultSceneRenderData->transform, glm::vec3(20.0f)));
		m_Shader->SetFloat4("u_Color", m_DefaultSceneRenderData->color);

		m_DefaultSceneRenderData->whiteTexture->Bind();
		if (m_EnableGrid)
			m_Grid->Draw(true);
	}

	void SceneLayer::BeginScene()
	{
		m_FrameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		RenderCommand::Clear();
		auto camera = MainCameraLayer::GetInstance()->GetCameraController().GetCamera();
		m_Shader->Bind();
		m_Shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void SceneLayer::EndScene()
	{
		m_FrameBuffer->Unbind();
	}

	void SceneLayer::OnUpdate(Timestep timestep)
	{
		BeginScene();
		DrawGrid();
		MainCameraLayer::GetInstance()->GetCameraController().OnUpdate(timestep);
		m_MainScene->OnUpdate(timestep, m_Shader);
		EndScene();
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
	}

	void SceneLayer::OnEvent(Event& event)
	{
		MainCameraLayer::GetInstance()->GetCameraController().OnEvent(event);
	}
}