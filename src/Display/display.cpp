#include "display.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include "../Camera/camera.h"

Display CreateDisplay(int width, int height, const char* title)
{
	// Init GLFW
	glfwInit();
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// TODO: REMOVE
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
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
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//glEnable(GL_BLEND);

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

void ProcessInput(Display& display, Camera& camera, bool rotating, bool& shouldReset)
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

		if(camera.firstClick)
		{
			display.mouseX = xpos;
			display.mouseY = ypos;
			camera.firstClick = false;
			return;
		}

		camera.yaw += (float)(xpos - display.mouseX);
		camera.pitch += (float)(display.mouseY - ypos);

		// Fixes able to look upside down
		if(camera.pitch > 89.0f)
			camera.pitch = 89.0f;
		if(camera.pitch < -89.0f)
			camera.pitch = -89.0f;

		camera.forward.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
		camera.forward.y = sin(glm::radians(camera.pitch));
		camera.forward.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
		camera.forward = glm::normalize(camera.forward);
		camera.position = -camera.forward * camera.cameraDistance;

		display.mouseX = xpos;
		display.mouseY = ypos;
	}
	else if(glfwGetMouseButton(display.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && !shouldReset)
		shouldReset = true;
}