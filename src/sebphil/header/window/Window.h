#pragma once
#include <cstdint>
#include <string>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

struct WindowMetaData {
	std::string title;
	uint32_t width;
	uint32_t height;
};

class Window {
	// TODO: when window resizes the user has to explicit tell the metadata to update in his own callback
	// TODO: window has to be activated on every frame!
	// TODO: Maybe add map & whenever resize callback is called, GLFWwindow* can be used as key in map to get a Window back as value
	// TODO: add clear function for Color-, Depth- & Stencilbuffer
private:
	GLFWwindow* window;

	static void resizeFramebuffer(GLFWwindow* window, int height, int width);

	void createWindow(const std::string& title, uint32_t width, uint32_t height);
	void setWindowHints();
	void createGlfwWindow(const std::string& title, uint32_t width, uint32_t height);

	void initializeGlContext();
	void loadGlBindings();
	void setUpOpenGL();

public:
	Window(const std::string& title, uint32_t width, uint32_t height);
	~Window();

	void focus() const;
	void clear() const;

	void setFramebufferSizeCallback(GLFWframebuffersizefun func);
	void setCursorPosCallback(GLFWcursorposfun func);
	void setKeyCallback(GLFWkeyfun func);

	GLFWwindow* getGlfwWindow() const;
	const unsigned int getWidth() const;
	const unsigned int getHeight() const;

};