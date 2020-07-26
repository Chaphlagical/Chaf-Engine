#pragma once

#include <Engine/core.h>
#include <string>

namespace Chaf
{
	class CHAF_API Texture
	{
	public:
		virtual ~Texture() = default;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;
		virtual std::string GetPathName() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class CHAF_API Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path, const bool hdr = false);
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
	};
}