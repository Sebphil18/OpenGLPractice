#pragma once
#include "glm/glm.hpp"

class Camera {

private:

	float yaw, pitch;
	float fieldOfView, far, near;
	unsigned int width, height;

	glm::vec3 cameraPosition;
	glm::vec3 cameraForward;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;
	glm::vec3 cameraTarget;
	glm::vec3 cameraDirection;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	void setUpCamera();
	void setUpMatrices();

	void setViewMatrix();
	void setProjectionMatrix();
	void setCameraTarget(glm::vec3 newTarget);
	void setCameraForwardAxis(glm::vec3 newForward);
	void setCameraDirection();
	void setCameraRightAxis();
	void setCameraUpAxis();
	
	glm::vec3 calcCameraDirection();
	glm::vec3 calcCameraRightAxis();
	glm::vec3 calcCameraUpAxis();
	glm::mat4 calcProjectionMatrix();
	glm::mat4 calcViewMatrix();
	void calcCameraForwardAxis();

public:

	float sensitivity, movementSpeed;

	Camera();
	Camera(unsigned int width, unsigned int height);
	
	void moveRight(float frameTime);
	void moveLeft(float frameTime);
	void moveForward(float frameTime);
	void moveBackwards(float frameTime);
	void moveUp(float frameTime);
	void moveDown(float frameTime);
	void moveYawAndPitch(float xoffset, float yoffset);

	void setCameraPosition(glm::vec3 newPosition);
	void setFieldOfView(float fieldOfView);
	void setWidth(unsigned int width);
	void setHeight(unsigned int height);

	glm::mat4 getProjectionMatrix() const;
	glm::mat4 getViewMatrix() const;
	glm::vec3 getPosition() const;
	glm::vec3 getForward() const;

};