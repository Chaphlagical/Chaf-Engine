#include "BezierCurve.h"

#include <cmath>
#include <iostream>

namespace Chaf
{
	inline uint32_t Cnk(uint32_t n, uint32_t k)
	{
		if (n == k || k == 0)
		{
			return 1;
		}

		std::vector<uint32_t> dp(k + 1);
		for (uint32_t i = 0; i <= n; i++)
		{
			for (int64_t j = static_cast<int64_t>(std::min(i, k)); j >= 0; j--)
			{
				if (i == j || j == 0)
				{
					dp[j] = 1;
				}
				else
				{
					dp[j] = dp[j] + dp[j - 1];
				}
			}
		}

		return dp[k];
	}

	inline double Bernstein(uint32_t n, uint32_t i, double t)
	{
		return Cnk(n, i) * std::pow(t, i) * std::pow(1 - t, n - i);
	}

	void BezierCurve::create(const std::vector<double>& input_x, const std::vector<double>& input_y, std::vector<double>& output_x, std::vector<double>& output_y, int samples)
	{
		double t = 0.0;
		auto n = input_x.size();
		output_x.resize(samples);
		output_y.resize(samples);

#pragma omp parallel for
		for (auto k = 0; k < samples; k++)
		{
			t = static_cast<double>(k) / static_cast<double>(samples);
			output_x[k] = 0;
			output_y[k] = 0;

#pragma omp parallel for
			for (uint32_t i = 0; i < n; i++)
			{
				output_x[k] += Bernstein(n - 1, i, t) * input_x[i];
				output_y[k] += Bernstein(n - 1, i, t) * input_y[i];
			}
		}
	}
}