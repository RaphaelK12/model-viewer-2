#pragma once
#include <glm/vec3.hpp>

struct Camera
{
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;

	float yaw, pitch;
	float firstClick;
};