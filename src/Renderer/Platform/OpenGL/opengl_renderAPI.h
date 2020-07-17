#pragma once

#include <Renderer/renderAPI.h>

namespace Chaf
{
	class CHAF_API OpenGLRendererAPI :public RenderAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetLineMode(const bool enable) override;
		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};

}