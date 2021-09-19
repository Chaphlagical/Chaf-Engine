#pragma once

#include "ColorEditing.h"

#include <DIP/DIP/Image.h>

#include <Engine/core.h>

namespace Chaf
{
	enum class ColorizationMethod
	{
		Global,
		Swatches
	};

	struct Region
	{
		cv::Point2i min;
		cv::Point2i max;
	};

	class Colorization :public ColorEditing
	{
	public:
		Colorization(const Ref<Image>& source, const Ref<Image>& target);

		~Colorization() = default;

		void setMethod(ColorizationMethod method);

		void addRegion(const Region& source, const Region& target);

		// Source: color image
		// Target: gray image
		virtual Ref<Image> solve() override;

	private:
		cv::Mat luminanceRemap(const cv::Mat& source, const cv::Mat& target);

		cv::Mat stddevNeiborhood(const cv::Mat& image, size_t neighborhood_size = 5);

		// Find best match for target from source
		cv::Point2i findBestMatch(double target_luminance, double target_stddev, const cv::Mat& source_luminance, const cv::Mat& source_stddev);

	private:
		ColorizationMethod m_method = ColorizationMethod::Global;
		std::vector<Region> m_src_regions;
		std::vector<Region> m_tar_regions;
	};
}