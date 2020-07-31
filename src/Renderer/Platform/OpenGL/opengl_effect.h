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
		virtual Ref<Texture2D>& GetTexture() { return m_Texture; };

		virtual void BindCubeMap(Camera& camera, uint32_t slot = 0) const override;
		virtual void BindIrradianceMap(uint32_t slot = 0) const override;
		virtual void BindPrefilterMap(uint32_t slot = 0) const override;
		virtual void BindBRDFLUTTMap(uint32_t slot = 0) const override;

	private:
		void GenCube();
		void GenQuad();
		void GenBuffer();
		void RenderCube() const;
		void RenderQuad() const;
	private:
		Ref<Texture2D> m_Texture;
		Ref<Shader> m_EquirectangularToCubemapShader;
		Ref<Shader> m_BackgroundShader;
		Ref<Shader> m_IrradianceShader;
		Ref<Shader> m_PrefilterShader;
		Ref<Shader> m_BRDFShader;
		uint32_t m_CubemapID = 0;
		uint32_t m_IrradianceMapID = 0;
		uint32_t m_PrefilterMapID = 0;
		uint32_t m_BRDFLUTTextureID = 0;
		uint32_t m_CubeVAO = 0, m_CubeVBO = 0;
		uint32_t m_QuadVAO = 0, m_QuadVBO = 0;
	};
}