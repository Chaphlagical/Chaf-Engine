#pragma once

#include <Engine/layer.h>
#include <Engine/time.h>
#include <Renderer/buffer.h>
#include <Renderer/texture.h>
#include <Renderer/vao.h>
#include <Editor/camera_controller.h>

namespace Chaf
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep timestep) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;
	private:
		CameraController m_CameraController;

		//	Temp
		Ref<FrameBuffer> m_FrameBuffer;
		Ref<VertexArray> m_VertexArray;
		glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.4f };

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		Ref<Texture2D> m_Texture;
	};
}