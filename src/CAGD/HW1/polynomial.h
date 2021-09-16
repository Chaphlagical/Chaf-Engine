#pragma once

#include "interpolation.h"

namespace Chaf
{
	class PolynomialInterpolation: public Interpolation
	{
	public:
		PolynomialInterpolation(const std::vector<glm::f64vec2>& points);

		~PolynomialInterpolation() = default;

		virtual double evaluate(double x) override;

		virtual std::vector<double> evaluate(const std::vector<double>& x) override;
	private:
		std::vector<double> m_params;
	};
}