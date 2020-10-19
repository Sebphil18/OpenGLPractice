#include "header/ShaderProgram.h"
#include <iostream>
#include <fstream>
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

ShaderProgram::ShaderProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile) {

    createProgram();

    vertexShader = getVertexShaderId(vertexShaderFile);
    fragmentShader = getFragmentShaderId(fragmentShaderFile);

    linkProgram();
    validateProgram();

    detachAllShader();
    deleteAllShader();

}

ShaderProgram::~ShaderProgram() {
    deleteProgram();
}

void ShaderProgram::createProgram() {
    glID = glCreateProgram();
}

unsigned int ShaderProgram::getVertexShaderId(const std::string& shaderFile) {

    std::string source = readFile(shaderFile);

    return compileShader(GL_VERTEX_SHADER, source);
}

unsigned int ShaderProgram::getFragmentShaderId(const std::string& shaderFile) {

    std::string source = readFile(shaderFile);

    return compileShader(GL_FRAGMENT_SHADER, source);
}

std::string ShaderProgram::readFile(const std::string& file) {

    std::ifstream fileInput(file);
    std::string src;

    if (fileInput.is_open())
        src = writeFileToString(fileInput);
    else
        std::cout << "ERROR::FILE::Could not open file " << file << std::endl;

    return src;
}

std::string ShaderProgram::writeFileToString(std::ifstream& fileInput) {

    std::string src;
    std::string line;

    while (std::getline(fileInput, line))
        src.append("\n" + line);

    return src;
}

unsigned int ShaderProgram::compileShader(unsigned int type, const std::string& source) {

    unsigned int shader = glCreateShader(type);

    const char* sourcePtr = source.c_str();

    glShaderSource(shader, 1, &sourcePtr, NULL);
    glCompileShader(shader);

    deleteInvalidShader(shader);

    return shader;
}

void ShaderProgram::deleteInvalidShader(unsigned int shader) {

    if (!shaderIsCompiled(shader)) {

        printShaderErrMsg(shader);
        deleteShader(shader);

    }
}

bool ShaderProgram::shaderIsCompiled(unsigned int shader) {

    int isCompiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

    return isCompiled;
}

void ShaderProgram::printShaderErrMsg(unsigned int shader) {

    std::string msg = getShaderErrMsg(shader);

    std::cout << "ERROR::SHADER::Could not compile shader: " << std::endl;
    std::cout << msg << std::endl;

}

std::string ShaderProgram::getShaderErrMsg(unsigned int shader) {

    int maxLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

    std::unique_ptr<char[]> msg = std::make_unique<char[]>(maxLength);

    glGetShaderInfoLog(shader, maxLength, &maxLength, msg.get());

    return std::string(msg.get());
}

void ShaderProgram::deleteShader(unsigned int shader) {
    glDeleteShader(shader);
}

void ShaderProgram::linkProgram() {
    glAttachShader(glID, vertexShader);
    glAttachShader(glID, fragmentShader);
    glLinkProgram(glID);
}

void ShaderProgram::validateProgram() {

    glValidateProgram(glID);

    if (!programIsLinked())
        std::cout << "ERROR::SHADERPROGRAM::Program could not be linked!" << std::endl;

    deleteInvalidProgram(programIsValid());

}

bool ShaderProgram::programIsLinked() {

    int isLinked;
    glGetProgramiv(glID, GL_LINK_STATUS, &isLinked);

    return isLinked;
}

bool ShaderProgram::programIsValid() {

    int isValid;
    glGetProgramiv(glID, GL_VALIDATE_STATUS, &isValid);

    return isValid;
}

void ShaderProgram::deleteInvalidProgram(bool isProgramValid) {

    if (!isProgramValid) {
        std::cout << "ERROR::SHADERPROGRAM::Program is not valid!" << std::endl;
        deleteProgram();
    }

}

void ShaderProgram::deleteProgram() {
    glDeleteProgram(glID);
    glID = 0;
}

void ShaderProgram::detachAllShader() {
    glDetachShader(glID, vertexShader);
    glDetachShader(glID, fragmentShader);
}

void ShaderProgram::deleteAllShader() {
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    vertexShader = 0;
    fragmentShader = 0;
}

void ShaderProgram::use() {
    glUseProgram(glID);
}

void ShaderProgram::bindUniformBuffer(unsigned int slot, const char* uniformBlock) {

    unsigned int index = glGetUniformBlockIndex(glID, uniformBlock);
    glUniformBlockBinding(glID, index, slot);

}

unsigned int ShaderProgram::getID() const {
    return glID;
}

void ShaderProgram::setUniform1b(const std::string& name, bool boolean) {
    setUniform1i(name, boolean);
}

void ShaderProgram::setUniform1i(const std::string& name, int i) {

    int location = getUniformLocation(name);

    glProgramUniform1i(glID, location, i);

    if (!uniformIsPresent(location))
        printUniformWarning(name);

}

void ShaderProgram::setUniform1f(const std::string& name, float decimal) {

    int location = getUniformLocation(name);

    glProgramUniform1f(glID, location, decimal);

    if (!uniformIsPresent(location))
        printUniformWarning(name);

}

void ShaderProgram::setUniformVec3f(const std::string& name, float x, float y, float z) {

    int location = getUniformLocation(name);

    glProgramUniform3f(glID, location, x, y, z);

    if (!uniformIsPresent(location))
        printUniformWarning(name);

}

void ShaderProgram::setUniformVec3f(const std::string& name, const glm::vec3& vec3) {

    int location = getUniformLocation(name);

    glProgramUniform3f(glID, location, vec3.x, vec3.y, vec3.z);

    if (!uniformIsPresent(location))
        printUniformWarning(name);

}

void ShaderProgram::setUniformVec4f(const std::string& name, float x, float y, float z, float w) {

    int location = getUniformLocation(name);

    glProgramUniform4f(glID, location, x, y, z, w);

    if (!uniformIsPresent(location))
        printUniformWarning(name);

}

void ShaderProgram::setUniformVec4f(const std::string& name, const glm::vec4& vec4) {

    int location = getUniformLocation(name);

    glProgramUniform4f(glID, location, vec4.x, vec4.y, vec4.z, vec4.w);

    if (!uniformIsPresent(location))
        printUniformWarning(name);
}

void ShaderProgram::setUniformMat4f(const std::string& name, const glm::mat4& matrix) {

    int location = getUniformLocation(name);

    glProgramUniformMatrix4fv(glID, location, 1, GL_FALSE, glm::value_ptr(matrix));

    if (!uniformIsPresent(location))
        printUniformWarning(name);

}

int ShaderProgram::getUniformLocation(std::string name) {

    if (isLocationCached(name))
        return uniformLocations[name];
    else {

        int location = glGetUniformLocation(glID, name.c_str());

        uniformLocations[name] = location;

        return location;
    }

}

bool ShaderProgram::isLocationCached(const std::string& name) {
    return uniformLocations.find(name) != uniformLocations.end();
}

bool ShaderProgram::uniformIsPresent(int location) {
    if (location == -1)
        return false;
    else
        return true;
}

void ShaderProgram::printUniformWarning(const std::string& name) {
    std::cout << "WARNING::SHADERPROGRAM::Using non existing uniform: " << name << std::endl;
}