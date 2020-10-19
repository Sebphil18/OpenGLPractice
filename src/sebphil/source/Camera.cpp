#include "header/Camera.h"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera() :
	movementSpeed(10), sensitivity(0.15), yaw(-90), pitch(0),
	fieldOfView(45), near(0.01), far(500), width(1), height(1) {

	setUpCamera();
	setUpMatrices();

}

Camera::Camera(unsigned int width, unsigned int height): 
	movementSpeed(10), sensitivity(0.15), yaw(-90), pitch(0),
	fieldOfView(45), near(0.01), far(500), width(width), height(height) {

	setUpCamera();
	setUpMatrices();

}

void Camera::setUpCamera() {

	setCameraPosition(glm::vec3(0, 0, 3));
	setCameraTarget(glm::vec3(0, 0, 0));
	setCameraDirection();

	setCameraForwardAxis(glm::vec3(0, 0, -1));
	setCameraRightAxis();
	setCameraUpAxis();

}

void Camera::setCameraPosition(glm::vec3 newPosition) {
	cameraPosition = newPosition;
}

void Camera::setCameraTarget(glm::vec3 newTarget) {
	cameraTarget = newTarget;
}

void Camera::setCameraDirection() {
	cameraDirection = calcCameraDirection();
}

glm::vec3 Camera::calcCameraDirection() {
	return cameraPosition - cameraTarget;
}

void Camera::setCameraForwardAxis(glm::vec3 newForward) {
	cameraForward = newForward;
}

void Camera::setCameraRightAxis() {
	cameraRight = calcCameraRightAxis();
}

glm::vec3 Camera::calcCameraRightAxis() {
	return glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameraDirection));
}

void Camera::setCameraUpAxis() {
	cameraUp = calcCameraUpAxis();
}

glm::vec3 Camera::calcCameraUpAxis() {
	return glm::normalize(glm::cross(cameraDirection, cameraRight));
}

void Camera::setUpMatrices() {
	setViewMatrix();
	setProjectionMatrix();
}

void Camera::setFieldOfView(float fieldOfView) {
	this->fieldOfView = fieldOfView;
	setProjectionMatrix();
}

void Camera::setProjectionMatrix() {
	projectionMatrix = calcProjectionMatrix();
}

glm::mat4 Camera::calcProjectionMatrix() {
	return glm::perspective(fieldOfView, (float)width / height, near, far);
}

void Camera::moveRight(float frameTime) {
	cameraPosition += glm::normalize(glm::cross(cameraForward, cameraUp)) * frameTime * movementSpeed;
	setViewMatrix();
}

void Camera::moveLeft(float frameTime) {
	cameraPosition -= glm::normalize(glm::cross(cameraForward, cameraUp)) * frameTime * movementSpeed;
	setViewMatrix();
}

void Camera::moveForward(float frameTime) {
	cameraPosition += cameraForward * movementSpeed * frameTime;
	setViewMatrix();
}

void Camera::moveBackwards(float frameTime) {
	cameraPosition -= cameraForward * movementSpeed * frameTime;
	setViewMatrix();
}

void Camera::moveUp(float frameTime) {
	cameraPosition += cameraUp * movementSpeed * frameTime;
	setViewMatrix();
}

void Camera::moveDown(float frameTime) {
	cameraPosition -= cameraUp * movementSpeed * frameTime;
	setViewMatrix();
}

void Camera::moveYawAndPitch(float xoffset, float yoffset) {

	yaw += xoffset * sensitivity;
	pitch += yoffset * sensitivity;

	if (pitch > 89.5)
		pitch = 89.5;
	else if (pitch < -89.5)
		pitch = -89.5;

	calcCameraForwardAxis();
	setViewMatrix();

}

void Camera::calcCameraForwardAxis() {

	cameraForward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraForward.y = sin(glm::radians(pitch));
	cameraForward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraForward = glm::normalize(cameraForward);

}

void Camera::setViewMatrix() {
	viewMatrix = calcViewMatrix();
}

glm::mat4 Camera::calcViewMatrix() {
	return glm::lookAt(cameraPosition, cameraForward + cameraPosition, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::setWidth(unsigned int width) {
	this->width = width;
	setProjectionMatrix();
}

void Camera::setHeight(unsigned int height) {
	this->height = height;
	setProjectionMatrix();
}

glm::vec3 Camera::getPosition() const {
	return cameraPosition;
}

glm::vec3 Camera::getForward() const {
	return cameraForward;
}

glm::mat4 Camera::getViewMatrix() const {
	return viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() const {
	return projectionMatrix;
}