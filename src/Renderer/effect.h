#pragma once

#include <Engine/core.h>
#include <Renderer/texture.h>
#include <Renderer/shader.h>
#include <Renderer/camera.h>

namespace Chaf
{
	class CHAF_API Cubemap
	{
	public:
		static Ref<Cubemap>Create(const std::string& path);
		static Ref<Cubemap>Create();
			
		virtual ~Cubemap() = default;
		virtual uint32_t GetCubemapID() const = 0;
		virtual uint32_t GetTextureID() const = 0;
		virtual std::string GetPathName() const = 0;
		virtual Ref<Texture2D>& GetTexture() = 0;
		
		virtual void Bind(Camera& camera, uint32_t slot = 0) const = 0;
	};
}