#pragma once

class Application {
	// Only one instance of this class should be created
	// TODO: may cause problems when multiple application instances are created

private:
	static bool glfwIsInitialized;

public:
	Application();
	~Application();
};
