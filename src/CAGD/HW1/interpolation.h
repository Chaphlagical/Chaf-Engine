#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Chaf
{
	class Interpolation
	{
	public:
		Interpolation() = default;

		~Interpolation() = default;

		virtual double evaluate(double x) = 0;

		virtual std::vector<double> evaluate(const std::vector<double>& x) = 0;
	};
}