#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct CameraComponent
{
	float Fov = 45.0f;
	float NearPlane = 0.1f;
	float FarPlane = 100.0f;
	glm::vec3 Offset = glm::vec3(0.0f, 0.0f, 7.0f);
};