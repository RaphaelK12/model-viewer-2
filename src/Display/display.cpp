#include "display.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "../Math/math.h"

Display CreateDisplay(int width, int height, const char* title)
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

	GLFWwindow* window = glfwCreateWindow(width, height, "2D Platformer", nullptr, nullptr);
	if(window == nullptr)
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// Initialize GLAD
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD!\n");
		glfwTerminate();
	}
	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	Display result { "", window, width, height, 0.0f, 0.0f, 0, 0.0f, 0.0, 0.0 };
	strcpy(result.title, title);
	return result;
}

void DeltaTimeCalc(Display& display)
{
	float currentTime = (float)glfwGetTime();
	display.deltaTime = currentTime - display.lastTime;
	display.lastTime = currentTime;
	display.numFrames++;

	// One second has passed
	if(currentTime - display.lastFPSTime >= 1.0f)
	{
		double frameTime = 1000.0 / (double)display.numFrames;
		double fps = (double)display.numFrames / (double)(currentTime - display.lastFPSTime);

		char buffer[1024];
		sprintf(buffer,"%s | FPS: %.2lf | Frame Time: %.3lf", display.title, fps, frameTime);
		glfwSetWindowTitle(display.window, buffer);

		display.numFrames = 0;
		display.lastFPSTime = currentTime;
	}
}

void ProcessInput(Display& display, glm::vec3& cameraRotation, bool rotating, bool& shouldReset)
{
	if(glfwGetMouseButton(display.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && rotating)
	{
		double xpos, ypos;
		glfwGetCursorPos(display.window, &xpos, &ypos);
		if(shouldReset)
		{
			display.mouseX = xpos;
			display.mouseY = ypos;
			shouldReset = false;
		}

		cameraRotation.y += (float)(xpos - display.mouseX);
		cameraRotation.x += (float)(ypos - display.mouseY);
		display.mouseX = xpos;
		display.mouseY = ypos;
	}
	else if(glfwGetMouseButton(display.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && !shouldReset)
		shouldReset = true;
}