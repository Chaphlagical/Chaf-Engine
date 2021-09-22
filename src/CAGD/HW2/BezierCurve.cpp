#include "BezierCurve.h"

namespace Chaf
{
	void BezierCurve::create(const std::vector<double>& input_x, const std::vector<double>& input_y, std::vector<double>& output_x, std::vector<double>& output_y)
	{
		double t = 0.0;
		output_x.clear();
		output_y.clear();
		for (double t = 0.0; t <= 1.0; t += 0.001)
		{
			std::vector<double> brx;
			std::vector<double> bry;
			std::vector<double> br_1x = input_x;
			std::vector<double> br_1y = input_y;
			for (auto r = 1; r < input_x.size(); r++)
			{
				brx.clear();
				bry.clear();
				for (auto i = 0; i < input_x.size() - r; i++)
				{
					brx.push_back((1 - t) * br_1x[i] + t * br_1x[i + 1]);
					bry.push_back((1 - t) * br_1y[i] + t * br_1y[i + 1]);
				}
				br_1x = brx;
				br_1y = bry;
			}
			output_x.push_back(brx[0]);
			output_y.push_back(bry[0]);
		}
	}
}