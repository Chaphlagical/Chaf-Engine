#pragma once

#include <vector>

namespace Chaf
{
	class BezierCurve
	{
	public:
		static void create(const std::vector<double>& input_x, const std::vector<double>& input_y, std::vector<double>& output_x, std::vector<double>& output_y);
	};
}