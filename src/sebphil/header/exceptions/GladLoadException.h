#pragma once
#include <exception>
#include <iostream>

class GladLoadException : public std::exception {
public:
	void printMsg() {
		std::cout << "EXCEPTION::Glad::Could not load OpenGL Bindings! \n";
	}
};