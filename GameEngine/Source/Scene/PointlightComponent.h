#pragma once
#include <glm/glm.hpp>

struct PointlightComponent
{
	glm::vec3 color;
	float intensity = 25.0;
	float radius = 15.0f;//Maximum distance it can reach
};