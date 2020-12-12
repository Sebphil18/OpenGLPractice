#include "window/RenderLoop.h"
#include "window/Window.h"

RenderLoop::RenderLoop(Window* window):
	window(window), glfwWindow(window->getGlfwWindow()), isRunning(false), 
	frame(0), timeAtLastFrame(0) {}

void RenderLoop::start() {
	isRunning = true;

	while (!glfwWindowShouldClose(glfwWindow) && isRunning) {
		update();
		render(window);

		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();

		frame++;
		timeAtLastFrame = glfwGetTime();
	}
}

void RenderLoop::update() {}
void RenderLoop::render(Window* window) {}

void RenderLoop::stop() {
	isRunning = false;
}

double RenderLoop::getFrameTime() const{
	double currentTime = glfwGetTime();
	return currentTime - timeAtLastFrame;
}

double RenderLoop::getFrameRate() const {
	double currentTime = glfwGetTime();
	double frameTime = currentTime - timeAtLastFrame;
	double frameRate = frameTime == 0 ? INFINITY : 1 / frameTime;
	return frameRate;
}

double RenderLoop::getFrameTime(RenderLoop* renderLoop) {
	double currentTime = glfwGetTime();
	return currentTime - renderLoop->timeAtLastFrame;
}