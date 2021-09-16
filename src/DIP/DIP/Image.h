#pragma once

#include <string>

#include <glad/glad.h>
#include <opencv2/opencv.hpp>

namespace Chaf
{
	class Image
	{
	public:
		Image(const std::string& path);

		Image(const cv::Mat& image);

		~Image();

		cv::Mat& getImage();

		GLuint getTextureID() const;

		const uint32_t getWidth() const;

		const uint32_t getHeight() const;

	private:
		std::string m_path = "";
		
		GLuint m_texture_id;
		cv::Mat m_image;

		uint32_t m_width = 0;
		uint32_t m_height = 0;
		
	};
}