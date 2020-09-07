#pragma once
#include "../Math/vec3.h"
// TEMP
#include <glm/vec3.hpp>

struct Camera
{
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;

	float yaw, pitch;
	bool firstClick;
	float cameraDistance;
};