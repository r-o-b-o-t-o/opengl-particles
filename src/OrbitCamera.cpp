#define _USE_MATH_DEFINES

#include <math.h>
#include "glm/gtc/matrix_transform.hpp"

#include "OrbitCamera.h"

float toRadians(float degrees) {
	return degrees * static_cast<float>(M_PI) / 180.0f;
}

OrbitCamera::OrbitCamera(float fov, float aspectRatio, float near, float far, glm::vec3 target, float distanceFromTarget, float minDist, float maxDist) :
		fov(fov),
		aspectRatio(aspectRatio),
		near(near),
		far(far),
		distance(distanceFromTarget),
		phi(toRadians(90.0f)),
		theta(glm::radians(20.0f)),
		target(target),
		wasMousePressed(false),
		lastMouseX(0.0f),
		lastMouseY(0.0f),
		sensitivityX(0.01f),
		sensitivityY(0.01f),
		minDist(minDist),
		maxDist(maxDist) {

	this->updateProjectionMatrix();
}

void OrbitCamera::update(GLFWwindow* window) {
	bool mousePressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
	if (mousePressed) {
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		double deltaX = 0.0;
		double deltaY = 0.0;

		if (this->wasMousePressed) {
			deltaX = this->lastMouseX - mouseX;
			deltaY = this->lastMouseY - mouseY;
		}

		this->phi -= static_cast<float>(deltaX) * this->sensitivityX;
		this->theta -= static_cast<float>(deltaY) * this->sensitivityY;

		this->lastMouseX = mouseX;
		this->lastMouseY = mouseY;
	}
	this->wasMousePressed = mousePressed;

	float maxTheta = 85.0f;
	this->theta = glm::clamp(this->theta, glm::radians(-maxTheta), glm::radians(maxTheta));
}

glm::mat4 OrbitCamera::getViewMatrix() const {
	glm::vec3 pos = this->getPosition();
	glm::vec3 target(0.0f, 0.0f, 0.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	return glm::lookAt(pos, target, up);
}

const glm::mat4 &OrbitCamera::getProjectionMatrix() const {
	return this->projectionMatrix;
}

glm::vec3 OrbitCamera::getPosition() const {
	return glm::vec3(
		this->distance * std::cos(this->theta) * std::cos(this->phi),
		this->distance * std::sin(this->theta),
		this->distance * std::cos(this->theta) * std::sin(this->phi)
	);
}

void OrbitCamera::zoom(float offset) {
	this->distance -= offset;
	this->distance = glm::clamp(this->distance, this->minDist, this->maxDist);
}

void OrbitCamera::setAspectRatio(float aspectRatio) {
	this->aspectRatio = aspectRatio;
	this->updateProjectionMatrix();
}

void OrbitCamera::setFov(float fov) {
	this->fov = fov;
	this->updateProjectionMatrix();
}

void OrbitCamera::setNear(float near) {
	this->near = near;
	this->updateProjectionMatrix();
}

void OrbitCamera::setFar(float far) {
	this->far = far;
	this->updateProjectionMatrix();
}

void OrbitCamera::updateProjectionMatrix() {
	this->projectionMatrix = glm::perspective(fov, aspectRatio, near, far);
}
