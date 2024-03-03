#include "depch.h"
#include "Math.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace OpenGLEngine::Math {
	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale)
	{
		// From glm::decompose in matrix_decompose.inl

		using namespace glm;
		using T = float;

		mat4 LocalMatrix(transform);

		// Normalize the matrix.
		if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
			return false;

		// First, isolate perspective.  This is the messiest.
		if (
			epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
		{
			// Clear the perspective partition
			LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
			LocalMatrix[3][3] = static_cast<T>(1);
		}

		// Next take care of translation (easy).
		translation = vec3(LocalMatrix[3]);
		LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

		vec3 Row[3];

		// Now get scale and shear.
		for (length_t i = 0; i < 3; ++i)
			for (length_t j = 0; j < 3; ++j)
				Row[i][j] = LocalMatrix[i][j];

		// Compute X scale factor and normalize first row.
		scale.x = length(Row[0]);
		Row[0] = detail::scale(Row[0], static_cast<T>(1));
		scale.y = length(Row[1]);
		Row[1] = detail::scale(Row[1], static_cast<T>(1));
		scale.z = length(Row[2]);
		Row[2] = detail::scale(Row[2], static_cast<T>(1));

		// At this point, the matrix (in rows[]) is orthonormal.
		// Check for a coordinate system flip.  If the determinant
		// is -1, then negate the matrix and the scaling factors.
#if 0
		vec3 Pdum3;
		Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
		if (dot(Row[0], Pdum3) < 0)
		{
			for (length_t i = 0; i < 3; i++)
			{
				scale[i] *= static_cast<T>(-1);
				Row[i] *= static_cast<T>(-1);
			}
		}
#endif

		rotation.y = asin(-Row[0][2]);
		if (cos(rotation.y) != 0) {
			rotation.x = atan2(Row[1][2], Row[2][2]);
			rotation.z = atan2(Row[0][1], Row[0][0]);
		}
		else {
			rotation.x = atan2(-Row[2][0], Row[1][1]);
			rotation.z = 0;
		}


		return true;
	}

	float MapRange(float value, float fromMin, float fromMax, float toMin, float toMax)
	{
		return (((value - fromMin) * (toMax - toMin)) / (fromMax - fromMin)) + toMin;
	}
}

namespace OpenGLEngine::Interpolator {
	glm::vec2 CatmullRomInterpolation(float t, const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3)
	{
		float t2 = t * t;
		float t3 = t2 * t;

		glm::vec2 v0 = (p2 - p0) * 0.5f;
		glm::vec2 v1 = (p3 - p1) * 0.5f;

		float h00 = 2.0f * t3 - 3.0f * t2 + 1.0f;
		float h01 = -2.0f * t3 + 3.0f * t2;
		float h10 = t3 - 2.0f * t2 + t;
		float h11 = t3 - t2;

		glm::vec2 interpolatedPoint = h00 * p1 + h10 * v0 + h01 * p2 + h11 * v1;

		return interpolatedPoint;
	}

	std::vector<glm::vec2> CatmullRomSplineInterpolation(const std::vector<glm::vec2>& points)
	{
		std::vector<glm::vec2> interpolatedPoints;

		for (size_t i = 1; i < points.size() - 2; ++i)
		{
			const glm::vec2& p0 = points[i - 1];
			const glm::vec2& p1 = points[i];
			const glm::vec2& p2 = points[i + 1];
			const glm::vec2& p3 = points[i + 2];

			for (float t = 0.0f; t <= 1.0f; t += 0.01f)
			{
				glm::vec2 interpolatedPoint = CatmullRomInterpolation(t, p0, p1, p2, p3);
				interpolatedPoints.push_back(interpolatedPoint);
			}
		}

		return interpolatedPoints;
	}

	glm::vec2 CubicInterpolation(float t, const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3)
	{
		float t2 = t * t;
		float t3 = t2 * t;

		float b0 = 0.5f * (-t3 + 2.0f * t2 - t);
		float b1 = 0.5f * (3.0f * t3 - 5.0f * t2 + 2.0f);
		float b2 = 0.5f * (-3.0f * t3 + 4.0f * t2 + t);
		float b3 = 0.5f * (t3 - t2);

		glm::vec2 interpolatedPoint = b0 * p0 + b1 * p1 + b2 * p2 + b3 * p3;

		return interpolatedPoint;
	}

	std::vector<glm::vec2> CubicSplineInterpolation(const std::vector<glm::vec2>& points)
	{
		std::vector<glm::vec2> interpolatedPoints;

		for (size_t i = 0; i < points.size() - 3; ++i)
		{
			const glm::vec2& p0 = points[i];
			const glm::vec2& p1 = points[i + 1];
			const glm::vec2& p2 = points[i + 2];
			const glm::vec2& p3 = points[i + 3];

			for (float t = 0.0f; t <= 1.0f; t += 0.01f)
			{
				glm::vec2 interpolatedPoint = CubicInterpolation(t, p0, p1, p2, p3);
				interpolatedPoints.push_back(interpolatedPoint);
			}
		}

		return interpolatedPoints;
	}

	glm::vec2 BezierInterpolation(float t, const std::vector<glm::vec2>& points)
	{
		int n = points.size() - 1;
		glm::vec2 interpolatedPoint(0.0f);

		for (int i = 0; i <= n; ++i)
		{
			float binomialCoeff = 1.0f;
			float ti = pow(1.0f - t, n - i) * pow(t, i);
			binomialCoeff *= ti;

			interpolatedPoint += binomialCoeff * points[i];
		}

		return interpolatedPoint;
	}

	std::vector<glm::vec2> BezierSplineInterpolation(const std::vector<glm::vec2>& points)
	{
		float numPoints = 1000.0f;

		std::vector<glm::vec2> interpolatedPoints;
		float stepSize = 1.0f / numPoints;

		for (int i = 0; i <= numPoints; ++i)
		{
			float t = i * stepSize;
			glm::vec2 interpolatedPoint = BezierInterpolation(t, points);
			interpolatedPoints.push_back(interpolatedPoint);
		}

		return interpolatedPoints;
	}

	glm::vec2 LinearInterpolation(float t, const glm::vec2& p0, const glm::vec2& p1)
	{
		glm::vec2 interpolatedPoint = p0 + t * (p1 - p0);
		return interpolatedPoint;
	}

	std::vector<glm::vec2> LinearInterpolation(const std::vector<glm::vec2>& points)
	{
		float numPoints = 1000.0f;

		std::vector<glm::vec2> interpolatedPoints;
		int numSegments = points.size() - 1;
		float stepSize = 1.0f / numPoints;

		for (int i = 0; i < numSegments; ++i)
		{
			const glm::vec2& p0 = points[i];
			const glm::vec2& p1 = points[i + 1];

			for (int j = 0; j <= numPoints; ++j)
			{
				float t = j * stepSize;
				glm::vec2 interpolatedPoint = LinearInterpolation(t, p0, p1);
				interpolatedPoints.push_back(interpolatedPoint);
			}
		}

		return interpolatedPoints;
	}
}