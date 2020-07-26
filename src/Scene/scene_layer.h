#pragma once

#include <Engine/core.h>
#include <Engine/layer.h>
#include <Scene/scene.h>
#include <Renderer/buffer.h>
#include <Renderer/shader.h>
#include <Renderer/mesh.h>
#include <Renderer/effect.h>
#include <glm/glm.hpp>
// TODO: Component System
namespace Chaf
{
	struct SceneRenderData
	{
		Ref<Texture2D> whiteTexture;
		Ref<Texture2D> checkboardTexture;
		glm::vec4 color;
		glm::mat4 transform;

		SceneRenderData()
		{
			whiteTexture = Texture2D::Create(1, 1);
			uint32_t defaultTextureData = 0xffffffff;
			whiteTexture->SetData(&defaultTextureData, sizeof(uint32_t));
			checkboardTexture = Texture2D::Create("assets/texture/checkboard.png");
			color = glm::vec4{ 1.0f };
			transform = glm::mat4{ 1.0f };
		}
	};

	class CHAF_API SceneLayer
		: public Layer
	{
	public:
		SceneLayer();
		~SceneLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override {};

		virtual void OnUpdate(Timestep timestep) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		void BeginScene();
		void EndScene();
		void DrawGrid();

		static SceneLayer* GetInstance() { return s_Instance; };
		Ref<Scene>& GetScene() { return m_MainScene; }
		Ref<Cubemap>& GetSkybox() { return m_Cubemap; }
		static Ref<SceneRenderData>& GetDefaultRenderData() { return m_DefaultSceneRenderData; }

		bool IsShowGrid() { return m_EnableGrid; }
		void SetShowGrid(const bool& enable) { m_EnableGrid = enable; }

	private:
		static SceneLayer* s_Instance;
		Ref<Scene> m_MainScene;

	private:
		Ref<Cubemap> m_Cubemap;
		Ref<FrameBuffer> m_FrameBuffer;
		Ref<Shader> m_Shader;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		Ref<TriMesh> m_Grid;
		bool m_EnableGrid = true;
		static Ref<SceneRenderData> m_DefaultSceneRenderData;
	};
}