#include <Engine/log.h>
#include <Engine/core.h>

#include <Renderer/Platform/OpenGL/opengl_texture.h>
#include <glm/glm.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Chaf
{
	/*OpenGLTexture2D*/
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path, const bool hdr)
		:m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		if (!hdr)
		{
			stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

			CHAF_CORE_ASSERT(data, "Failed to load image!");
			m_Width = width;
			m_Height = height;

			GLenum internalFormat = 0;

			switch (channels)
			{
			case 1:
				m_Format = GL_RED;
				internalFormat = GL_RED;
				break;
			case 3:
				m_Format = GL_RGB;
				internalFormat = GL_RGB8;
				break;
			case 4:
				m_Format = GL_RGBA;
				internalFormat = GL_RGBA8;
				break;
			default:
				CHAF_CORE_ASSERT(false, "Unknown image channels m_Format!");
				m_Format = 0;
				break;
			}

			glGenTextures(1, &m_RendererID);
			glBindTexture(GL_TEXTURE_2D, m_RendererID);
			//glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);
			glTexImage2D(GL_TEXTURE_2D, 0, m_Format, width, height, 0, m_Format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			//glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_Format, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}
		else
		{
			float* data = stbi_loadf(path.c_str(), &width, &height, &channels, 0);

			if (data)
			{
				glGenTextures(1, &m_RendererID);
				glBindTexture(GL_TEXTURE_2D, m_RendererID);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				stbi_image_free(data);
			}
			else
			{
				CHAF_CORE_ASSERT(false, "Texture load failure!");
			}
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		:m_Width(width), m_Height(height), m_Path("")
	{
		GLenum internalFormat = GL_RGBA8;
		m_Format = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t channelNum = m_Format == GL_RGBA ? 4 : 3;
		CHAF_CORE_ASSERT(size == m_Width * m_Height * channelNum, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_Format, GL_UNSIGNED_BYTE, data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}