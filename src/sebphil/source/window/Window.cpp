#include "window/Window.h"
#include "exceptions/WindowCreationException.h"
#include "exceptions/GladLoadException.h"

void Window::resizeFramebuffer(GLFWwindow* window, int width, int height) {
	glfwMakeContextCurrent(window);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
}

Window::Window(const std::string& title, uint32_t width, uint32_t height) {
	createWindow(title, width, height);
}

void Window::createWindow(const std::string& title, uint32_t width, uint32_t height) {
	setWindowHints();
	createGlfwWindow(title, width, height);
	initializeGlContext();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::setWindowHints() {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);
}

void Window::createGlfwWindow(const std::string& title, uint32_t width, uint32_t height) {
	try {

		window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		if (window == nullptr)
			throw WindowCreationException(title);

	} catch (WindowCreationException& e) {
		e.printMsg();
	}
}

void Window::initializeGlContext() {
	glfwMakeContextCurrent(window);
	loadGlBindings();
	setUpOpenGL();
}

void Window::loadGlBindings() {
	try {
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			throw GladLoadException();
	} catch (GladLoadException& e) {
		e.printMsg();
	}
}

void Window::setUpOpenGL() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_MULTISAMPLE);
}

Window::~Window() {
	glfwDestroyWindow(window);
}

void Window::focus() const {
	glfwMakeContextCurrent(window);
}

void Window::clear() const {
	glfwMakeContextCurrent(window);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_STENCIL_BUFFER_BIT);
}

void Window::setFramebufferSizeCallback(GLFWframebuffersizefun func) {
	glfwSetFramebufferSizeCallback(window, func);
}

void Window::setCursorPosCallback(GLFWcursorposfun func) {
	glfwSetCursorPosCallback(window, func);
}

void Window::setKeyCallback(GLFWkeyfun func) {
	glfwSetKeyCallback(window, func);
}

GLFWwindow* Window::getGlfwWindow() const {
	return window;
}

const unsigned int Window::getWidth() const {
	int width = 0;
	int height = 0;
	glfwGetWindowSize(window, &width, &height);
	return width;
}

const unsigned int Window::getHeight() const {
	int width = 0;
	int height = 0;
	glfwGetWindowSize(window, &width, &height);
	return height;
}