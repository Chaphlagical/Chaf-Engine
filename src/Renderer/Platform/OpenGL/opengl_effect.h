#pragma once

#include <Renderer/effect.h>
#include <Renderer/texture.h>
#include <Renderer/shader.h>
#include <Renderer/buffer.h>
#include <Renderer/vao.h>
#include <Renderer/camera.h>
#include <glad/glad.h>

namespace Chaf
{
	class CHAF_API OpenGLCubemap : public Cubemap
	{
	public:
		OpenGLCubemap(const std::string& path);
		OpenGLCubemap();
		virtual ~OpenGLCubemap();
		virtual uint32_t GetCubemapID() const override { return m_CubemapID; }
		virtual uint32_t GetTextureID() const override { return m_Texture->GetRendererID(); }
		virtual std::string GetPathName() const override { return m_Texture->GetPathName(); }

		virtual void Bind(Camera& camera, uint32_t slot = 0) const override;

	private:
		void GenCube();
		void GenBuffer();
	private:
		Ref<Texture2D> m_Texture;
		Ref<Shader> m_EquirectangularToCubemapShader;
		Ref<Shader> m_BackgroundShader;
		uint32_t m_CubemapID = 0;
		uint32_t m_CubeVAO = 0, m_CubeVBO = 0;
	};
}