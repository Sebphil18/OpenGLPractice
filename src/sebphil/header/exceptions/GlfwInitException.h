#pragma once
#include <exception>
#include <iostream>

class GlfwInitException : public std::exception {
public:
	void printMsg() {
		std::cout << "EXCEPTION::GLFW::Could not initiate GLFW! \n";
	}
};
