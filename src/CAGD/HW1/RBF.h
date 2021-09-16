#pragma once

#include "interpolation.h"

namespace Chaf
{
	class RBFInterpolation: public Interpolation
	{
	public:
		RBFInterpolation(const std::vector<glm::f64vec2>& points, double d);

		~RBFInterpolation() = default;

		virtual double evaluate(double x) override;

		virtual std::vector<double> evaluate(const std::vector<double>&x) override;

	private:
		double m_constant = 0.f;
		double m_d = 0.f;
		std::vector<double> m_params;
		std::vector<double> m_ps;
	};
}