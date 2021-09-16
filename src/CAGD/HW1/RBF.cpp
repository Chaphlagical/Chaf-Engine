#include "RBF.h"

#include <Eigen/Eigen>

namespace Chaf
{
	RBFInterpolation::RBFInterpolation(const std::vector<glm::f64vec2>& points, double d) :
		m_d(d)
	{
		Eigen::VectorXd y(points.size());
		Eigen::MatrixXd A(points.size(), points.size());
		m_ps.resize(points.size());
		m_params.resize(points.size());

		m_constant = 0.f;

		for (uint32_t i = 0; i < points.size(); i++)
		{
			m_ps[i] = points[i].x;
			y(i) = points[i].y;
			m_constant += points[i].y;
		}

		m_constant /= static_cast<double>(points.size());
		y = y.array() - m_constant;

		for (uint32_t i = 0; i < points.size(); i++)
		{
			for (uint32_t j = 0; j < points.size(); j++)
			{
				A(i, j) = 1.f / ((points[i].x - m_ps[j]) * (points[i].x - m_ps[j]) + m_d);
			}
		}

		Eigen::VectorXd x = A.colPivHouseholderQr().solve(y);

		for (uint32_t i = 0; i < points.size(); i++)
		{
			m_params[i] = x(i);
		}
	}

	double RBFInterpolation::evaluate(double x)
	{
		double result = m_constant;

		for (uint32_t i = 0; i < m_params.size(); i++)
		{
			result += m_params[i] / ((x - m_ps[i]) * (x - m_ps[i]) + m_d);
		}

		return result;
	}

	std::vector<double> RBFInterpolation::evaluate(const std::vector<double>& x)
	{
		std::vector<double> result(x.size());

		for (uint32_t i = 0; i < result.size(); i++)
		{
			result[i] = evaluate(x[i]);
		}
		return result;
	}
}