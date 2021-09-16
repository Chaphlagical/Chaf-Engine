#include "polynomial.h"
#include <Eigen/Eigen>

namespace Chaf
{
	PolynomialInterpolation::PolynomialInterpolation(const std::vector<glm::f64vec2>& points)
	{
		// Ax=y, we want x
		size_t dim = points.size();
		Eigen::MatrixXd A(dim, dim);
		Eigen::VectorXd y(dim);
		m_params.resize(dim);

		for (uint32_t i = 0; i < points.size(); i++)
		{
			for (uint32_t j = 0; j < points.size(); j++)
			{
				A(i, j) = pow(points[i].x, (double)j);
			}

			y(i) = points[i].y;
		}

		Eigen::VectorXd x = A.colPivHouseholderQr().solve(y);

		for (uint32_t i = 0; i < points.size(); i++)
		{
			m_params[i] = x(i);
		}
	}

	double PolynomialInterpolation::evaluate(double x)
	{
		double result = 0.f;

		for (uint32_t i = 0; i < m_params.size(); i++)
		{
			result += m_params[i] * std::pow(x, static_cast<double>(i));
		}

		return result;
	}

	std::vector<double> PolynomialInterpolation::evaluate(const std::vector<double>& x)
	{
		std::vector<double> result(x.size());

		for (uint32_t i = 0; i < result.size(); i++)
		{
			result[i] = evaluate(x[i]);
		}
		return result;
	}
}