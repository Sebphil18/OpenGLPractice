#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include "header/UniformBuffer.h"

enum class ShaderType {vertex, geometry, fragment};

class ShaderProgram {

	// TODO: rework getShaderID (every shader should work)
	// TODO: add support for geometry shader

private:
	unsigned int glID, vertexShader, geometryShader, fragmentShader;

	std::unordered_map<std::string, int> uniformLocations;

	int getUniformLocation(std::string name);
	
	void buildProgram();
	unsigned int compileShader(unsigned int type, const std::string& source);

	std::string readFile(const std::string& file);

	void createProgram();
	uint32_t getShaderId(const std::string& shaderFile, ShaderType shaderType);
	uint32_t getShaderTypeBinding(ShaderType shaderType);
	void linkProgram();
	void validateProgram();
	bool programIsLinked();
	bool programIsValid();
	void deleteInvalidProgram(bool isProgramValid);
	void deleteProgram();
	void detachAllShader();
	void deleteAllShader();
	void deleteShader(unsigned int shader);
	void deleteInvalidShader(unsigned int shader);
	bool shaderIsCompiled(unsigned int shader);
	std::string getShaderErrMsg(unsigned int shader);
	void printShaderErrMsg(unsigned int shader);
	std::string writeFileToString(std::ifstream& fileInput);
	void printUniformWarning(const std::string& name);
	bool uniformIsPresent(int location);
	bool isLocationCached(const std::string& name);

public:
	ShaderProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
	ShaderProgram(const std::string& vertexShaderFile, const std::string& gemShaderFile, const std::string& fragmentShaderFile);
	~ShaderProgram();

	void use();

	void bindUniformBuffer(unsigned int slot, const char* uniformBlock);

	void setUniform1i(const std::string& name, int integer);
	void setUniform1b(const std::string& name, bool boolean);
	void setUniform1f(const std::string& name, float decimal);

	void setUniformVec3f(const std::string& name, float x, float y, float z);
	void setUniformVec3f(const std::string& name, const glm::vec3& vec3);
	void setUniformVec4f(const std::string& name, float x, float y, float z, float w);
	void setUniformVec4f(const std::string& name, const glm::vec4& vec4);

	void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

	unsigned int getID() const;

};