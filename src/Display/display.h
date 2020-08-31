#pragma once
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct Display
{
	char title[512];
	struct GLFWwindow* window;
	int width, height;

	float deltaTime = 0.0f;
	float lastTime = 0.0f;

	// Used for FPS calculations
	int numFrames = 0;
	float lastFPSTime = 0;

	double mouseX, mouseY;
};

Display CreateDisplay(int width, int height, const char* title);
void DeltaTimeCalc(Display& display);
void ProcessInput(Display& display, struct Camera& camera, bool rotating, bool& shouldReset);