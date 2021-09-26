#include "BezierCurve.h"

namespace Chaf
{
	void BezierCurve::create(const std::vector<double>& input_x, const std::vector<double>& input_y, std::vector<double>& output_x, std::vector<double>& output_y, int samples)
	{
		double t = 0.0;
		int n = samples;
		output_x.resize(n);
		output_y.resize(n);

#pragma omp parallel for
		for (auto k = 0; k < n; k++)
		{
			double t = static_cast<double>(k) / static_cast<double>(n);
			std::vector<double> brx(input_x);
			std::vector<double> bry(input_x);
			std::vector<double> br_1x = input_x;
			std::vector<double> br_1y = input_y;
			for (auto r = 1; r < input_x.size(); r++)
			{
#pragma omp parallel for
				for (auto i = 0; i < input_x.size() - r; i++)
				{
					brx[i] = (1 - t) * br_1x[i] + t * br_1x[i + 1];
					bry[i] = (1 - t) * br_1y[i] + t * br_1y[i + 1];
				}
				br_1x = brx;
				br_1y = bry;
			}
			output_x[k] = brx[0];
			output_y[k] = bry[0];
		}
	}
}