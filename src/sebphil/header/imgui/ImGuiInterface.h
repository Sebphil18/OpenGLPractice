#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"

class ImGuiInterface {

private:
	GLFWwindow* window;

	void init(GLFWwindow* window);
	void shutdown();

public:
	ImGuiInterface(GLFWwindow* window);
	~ImGuiInterface();

	void newFrame();
	void render();

	float getFrameTime() const;

};
