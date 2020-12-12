#include "window/Application.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <exceptions/GlfwInitException.h>

bool Application::glfwIsInitialized = false;

Application::Application() {
	try {
		if (glfwInit() == GLFW_FALSE)
			throw GlfwInitException();
		else
			glfwIsInitialized = true;
	} catch (GlfwInitException& e) {
		e.printMsg();
	}
}

Application::~Application() {
	if (glfwIsInitialized) {
		glfwTerminate();
		glfwIsInitialized = false;
	}
}