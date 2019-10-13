#ifndef ORBIT_CAMERA_H
#define ORBIT_CAMERA_H

#include "GLFW/glfw3.h"
#include "glm/matrix.hpp"

class OrbitCamera {
public:
	OrbitCamera(float fov, float aspectRatio,
				float near = 0.1f, float far = 100.0f,
				glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f), float distanceFromTarget = 10.0f,
				float minDist = 0.5f, float maxDist = 20.0f);

	void update(GLFWwindow* window);
	glm::mat4 getViewMatrix() const;
	const glm::mat4 &getProjectionMatrix() const;
	glm::vec3 getPosition() const;

	void zoom(float offset);
	void setAspectRatio(float aspectRatio);
	void setFov(float fov);
	void setNear(float near);
	void setFar(float far);

private:
	void updateProjectionMatrix();

	float distance;
	float phi;
	float theta;
	float fov;
	float aspectRatio;
	float near;
	float far;
	glm::vec3 target;
	glm::mat4 projectionMatrix;

	double lastMouseX;
	double lastMouseY;
	bool wasMousePressed;

	float sensitivityX;
	float sensitivityY;

	float minDist;
	float maxDist;
};

#endif