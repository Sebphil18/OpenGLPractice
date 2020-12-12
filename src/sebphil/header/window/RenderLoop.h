#pragma once
#include "Window.h"

class RenderLoop {

private:
	bool isRunning;
	unsigned long frame;
	double timeAtLastFrame;

protected:
	Window* window;
	GLFWwindow* glfwWindow;

	virtual void render(Window* window);
	virtual void update();

public:
	RenderLoop(Window* window);
	void start();
	void stop();

	double getFrameTime() const;
	double getFrameRate() const;

	static double getFrameTime(RenderLoop* renderLoop);

};