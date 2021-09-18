#include "Image.h"

#include <stb_image.h>

namespace Chaf
{
	inline unsigned char* Mat_to_Byte(const cv::Mat img)
	{
		unsigned char* ptr = new unsigned char[long long(img.rows) * long long(img.cols) * 4];
		size_t bia = 0;
		for (size_t i = 0; i < img.rows; i++)
		{
			for (size_t j = 0; j < img.cols; j++)
			{
				for (int k = 2; k >= 0; k--)
				{
					*(ptr + bia) = img.at<cv::Vec3b>(i, j)[k];
					bia++;
				}
				*(ptr + bia) = 255;
				bia++;
			}
		}
		return ptr;
	}


	Image::Image(const std::string& path)
	{
		m_image = cv::imread(path);
		cv::flip(m_image, m_image, 0);

		m_path = path;

		m_width = m_image.cols;
		m_height = m_image.rows;

		// Load from file
		unsigned char* image_data = Mat_to_Byte(m_image);

		if (image_data == NULL)
			return;

		// Create a OpenGL texture identifier
		glGenTextures(1, &m_texture_id);
		glBindTexture(GL_TEXTURE_2D, m_texture_id);

		// Setup filtering parameters for display
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Upload pixels into texture
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image.cols, m_image.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
		stbi_image_free(image_data);
	}

	Image::Image(const cv::Mat& image)
	{
		m_image = image.clone();

		m_width = m_image.cols;
		m_height = m_image.rows;

		// Load from file
		unsigned char* image_data = Mat_to_Byte(m_image);

		if (image_data == NULL)
			return;

		// Create a OpenGL texture identifier
		glGenTextures(1, &m_texture_id);
		glBindTexture(GL_TEXTURE_2D, m_texture_id);

		// Setup filtering parameters for display
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Upload pixels into texture
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image.cols, m_image.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
		stbi_image_free(image_data);
	}

	Image::~Image()
	{
		glDeleteTextures(1, &m_texture_id);
		m_image.release();
	}

	cv::Mat& Image::getImage()
	{
		return m_image;
	}

	GLuint Image::getTextureID() const
	{
		return m_texture_id;
	}

	const uint32_t Image::getWidth() const
	{
		return m_width;
	}

	const uint32_t Image::getHeight() const
	{
		return m_height;
	}

	const std::string& Image::getPath() const
	{
		return m_path;
	}

}