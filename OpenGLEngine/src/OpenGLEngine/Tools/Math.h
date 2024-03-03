#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace OpenGLEngine::Math {
	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
	float MapRange(float value, float fromMin, float fromMax, float toMin, float toMax);
}

namespace OpenGLEngine::Interpolator {
	std::vector<glm::vec2> CatmullRomSplineInterpolation(const std::vector<glm::vec2>& points);
	std::vector<glm::vec2> CubicSplineInterpolation(const std::vector<glm::vec2>& points);
	std::vector<glm::vec2> BezierSplineInterpolation(const std::vector<glm::vec2>& points);
	std::vector<glm::vec2> LinearInterpolation(const std::vector<glm::vec2>& points);

	glm::vec2 CatmullRomInterpolation(float t, const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3);
	glm::vec2 CubicInterpolation(float t, const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3);
	glm::vec2 BezierInterpolation(float t, const std::vector<glm::vec2>& points);
	glm::vec2 LinearInterpolation(float t, const glm::vec2& p0, const glm::vec2& p1);
}