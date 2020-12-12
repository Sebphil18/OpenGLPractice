#pragma once
#include <exception>
#include <iostream>
#include "window/Window.h"

class WindowCreationException : public std::exception {
private:
	std::string title;
public:
	WindowCreationException(const std::string& title) {
		this->title = title;
	}
	void printMsg() {
		std::cout << "EXCEPTION::GLFW::Could not create window '" << title << "'! (glfw has to be initilized before window can be created!)\n";
	}

};