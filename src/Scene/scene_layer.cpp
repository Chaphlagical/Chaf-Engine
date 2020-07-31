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
	std::function<void(glm::vec2)> SceneLayer::m_GuizmoFunc = [](glm::vec2) {};

	SceneLayer::SceneLayer()
	{
		s_Instance = this;
		m_MainScene = CreateRef<Scene>();
		m_DefaultSceneRenderData = CreateRef<SceneRenderData>();
	}

	void SceneLayer::OnAttach()
	{
		m_Grid=TriMesh::Create(MeshType::Plane, 10);

		RenderCommand::Init();

		FrameBufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(fbSpec);

		m_WireFrameShader = Shader::Create("assets/shader/environment/grid.glsl");

		m_Cubemap = Cubemap::Create();

		auto entity = m_MainScene->CreateEntity("light");
		auto& light = entity.AddComponent<LightComponent>();
		entity.AddComponent<MeshComponent>(MeshType::Sphere);
		entity.GetComponent<TransformComponent>().Position = glm::vec3{ 0.0f,3.0f,0.0f };
		CHAF_INFO(entity.HasComponent<LightComponent>());
		
		auto cube = m_MainScene->CreateEntity("cube");
		//cube.AddComponent<MeshComponent>(MeshType::Cube);
		//auto material = cube.AddComponent<MaterialComponent>(MaterialType::Material_Phong);

		//CastRef<PhongMaterial>(material.MaterialSrc)->ResetTexture(CastRef<PhongMaterial>(material.MaterialSrc)->DiffuseTexture, "assets/texture/container2.png");
		//CastRef<PhongMaterial>(material.MaterialSrc)->ResetTexture(CastRef<PhongMaterial>(material.MaterialSrc)->SpecularTexture, "assets/texture/container2_specular.png");

		cube.AddComponent<MeshComponent>(MeshType::Sphere, 10);
		auto material = cube.AddComponent<MaterialComponent>(MaterialType::Material_Cook_Torrance);
		CastRef<CookTorranceBRDF>(material.MaterialSrc)->ResetTexture(CastRef<CookTorranceBRDF>(material.MaterialSrc)->AlbedoTexture, "assets/texture/pbr/rustediron2_basecolor.png");
		CastRef<CookTorranceBRDF>(material.MaterialSrc)->ResetTexture(CastRef<CookTorranceBRDF>(material.MaterialSrc)->NormalTexture, "assets/texture/pbr/rustediron2_normal.png");
		CastRef<CookTorranceBRDF>(material.MaterialSrc)->ResetTexture(CastRef<CookTorranceBRDF>(material.MaterialSrc)->MetallicTexture, "assets/texture/pbr/rustediron2_metallic.png");
		CastRef<CookTorranceBRDF>(material.MaterialSrc)->ResetTexture(CastRef<CookTorranceBRDF>(material.MaterialSrc)->RoughnessTexture, "assets/texture/pbr/rustediron2_roughness.png");

	}

	void SceneLayer::DrawGrid()
	{
		m_WireFrameShader->Bind();
		m_WireFrameShader->SetMat4("u_ViewProjection", MainCameraLayer::GetInstance()->GetCameraController().GetCamera().GetViewProjectionMatrix());
		m_WireFrameShader->SetMat4("u_Transform", glm::scale(m_DefaultSceneRenderData->transform, glm::vec3(20.0f)));
		m_DefaultSceneRenderData->whiteTexture->Bind();
		if (m_EnableGrid)
			m_Grid->Draw(true);
	}

	void SceneLayer::BeginScene()
	{
		m_FrameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		RenderCommand::Clear();
		DrawGrid();
		auto camera = MainCameraLayer::GetInstance()->GetCameraController().GetCamera();
	}

	void SceneLayer::EndScene()
	{
		if (!SceneLayer::GetInstance()->GetScene()->GetLineMode())
		{
			RenderCommand::SetLineMode(false);
			m_Cubemap->Bind(MainCameraLayer::GetInstance()->GetCameraController().GetCamera());
		}
		m_FrameBuffer->Unbind();
	}

	void SceneLayer::OnUpdate(Timestep timestep)
	{
		BeginScene();
		MainCameraLayer::GetInstance()->GetCameraController().OnUpdate(timestep);
		m_MainScene->RenderObject(MainCameraLayer::GetInstance()->GetCameraController().GetCamera());
		
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
		m_GuizmoFunc(m_ViewportSize);
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