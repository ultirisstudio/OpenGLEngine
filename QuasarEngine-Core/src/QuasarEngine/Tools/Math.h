#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace QuasarEngine::Math {
	enum Direction
	{
		DIRECTION_LEFT,
		DIRECTION_RIGHT,
		DIRECTION_UP,
		DIRECTION_DOWN,
		DIRECTION_FORWARD,
		DIRECTION_BACKWARD,

		DIRECTION_COUNT
	};

	const glm::vec3 directionVectors[DIRECTION_COUNT]
	{
		{ 1, 0, 0 },
		{ -1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, -1, 0 },
		{ 0, 0, 1 },
		{ 0, 0, -1 },
	};

	struct Plane
	{
		float a, b, c, d;
	};

	struct Frustum
	{
		Plane planes[Math::DIRECTION_COUNT];
	};

	float MapRange(float value, float fromMin, float fromMax, float toMin, float toMax);

	Frustum CalculateFrustum(const glm::mat4& camera);
}

namespace QuasarEngine::Interpolator {
	std::vector<glm::vec2> CatmullRomSplineInterpolation(const std::vector<glm::vec2>& points);
	std::vector<glm::vec2> CubicSplineInterpolation(const std::vector<glm::vec2>& points);
	std::vector<glm::vec2> BezierSplineInterpolation(const std::vector<glm::vec2>& points);
	std::vector<glm::vec2> LinearInterpolation(const std::vector<glm::vec2>& points);

	glm::vec2 CatmullRomInterpolation(float t, const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3);
	glm::vec2 CubicInterpolation(float t, const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3);
	glm::vec2 BezierInterpolation(float t, const std::vector<glm::vec2>& points);
	glm::vec2 LinearInterpolation(float t, const glm::vec2& p0, const glm::vec2& p1);
}