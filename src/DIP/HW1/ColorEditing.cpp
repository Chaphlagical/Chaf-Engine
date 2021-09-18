#include "ColorEditing.h"

namespace Chaf
{
	ColorEditing::ColorEditing(const Ref<Image>& source, const Ref<Image>& target) :
		m_source(source),
		m_target(target)
	{
	}

	cv::Vec3d ColorEditing::RGB2lab(const cv::Vec3d& rgb)
	{
		cv::Vec3d LMS = cv::Matx33d(
			0.3811, 0.5783, 0.0402,
			0.1967, 0.7244, 0.0782,
			0.0241, 0.1288, 0.844) * rgb;

		LMS.val[0] = LMS.val[0] == 0.0 ? 1.0 : LMS.val[0];
		LMS.val[1] = LMS.val[1] == 0.0 ? 1.0 : LMS.val[1];
		LMS.val[2] = LMS.val[2] == 0.0 ? 1.0 : LMS.val[2];

		cv::log(LMS, LMS);

		LMS /= log(10);

		return
			cv::Matx33d(
				1 / std::sqrt(3.0), 0.0, 0.0,
				0.0, 1 / std::sqrt(6.0), 0.0,
				0.0, 0.0, 1 / std::sqrt(2.0)) *
			cv::Matx33d(
				1.0, 1.0, 1.0,
				1.0, 1.0, -2.0,
				1.0, -1.0, 0.0) *
			LMS;
	}

	cv::Vec3d ColorEditing::lab2RGB(const cv::Vec3d& lab)
	{
		cv::Vec3d LMS =
			cv::Matx33d(
				1.0, 1.0, 1.0,
				1.0, 1.0, -1.0,
				1.0, -2.0, 0.0) *
			cv::Matx33d(
				std::sqrt(3.0) / 3.0, 0.0, 0.0,
				0.0, std::sqrt(6.0) / 6.0, 0.0,
				0.0, 0.0, std::sqrt(2.0) / 2.0) * lab;

		LMS = cv::Vec3d(std::pow(10, LMS.val[0]), std::pow(10, LMS.val[1]), std::pow(10, LMS.val[2]));

		return cv::Matx33d(4.4679, -3.5873, 0.1193, -1.2186, 2.3809, -0.1624, 0.0497, -0.2439, 1.2045) * LMS;
	}
}