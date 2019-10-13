#include "Application.h"

Application::Application(int screenWidth, int screenHeight) :
		debugWindow(this),
		planeWindow(this),
		particlesWindow(this),
		screenWidth(screenWidth),
		screenHeight(screenHeight),
		camera(45.0f, 16.0f / 9.0f, 0.1f, 200.0f, glm::vec3(0.0f, 0.0f, 0.0f), 25.0f, 0.5f, 150.0f) {

	this->camera.setAspectRatio(static_cast<float>(this->screenWidth) / static_cast<float>(this->screenHeight));
	this->confettiEmitter.initialize();
	this->snowflakeEmitter.initialize();
	this->fireEmitter.initialize();
	this->activeEmitter = &this->confettiEmitter;
}

void Application::draw() {
	this->plane.getShader().setMat4("view", this->camera.getViewMatrix());
	this->plane.getShader().setMat4("projection", this->camera.getProjectionMatrix());
	this->plane.draw();

	this->drawUi();
}

void Application::drawUi() {
	this->debugWindow.draw();
	this->planeWindow.draw();
	this->particlesWindow.draw();
}

void Application::update(GLFWwindow* window, float deltaTime) {
	this->camera.update(window);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void Application::onWindowSizeChanged(int width, int height) {
	this->screenWidth = width;
	this->screenHeight = height;
	glViewport(0, 0, width, height);
	this->camera.setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
}

void Application::drawParticles() {
	this->activeEmitter->getShader().setMat4("view", this->camera.getViewMatrix());
	this->activeEmitter->getShader().setMat4("projection", this->camera.getProjectionMatrix());
	this->activeEmitter->draw();
}

int Application::getScreenWidth() const {
	return this->screenWidth;
}

int Application::getScreenHeight() const {
	return this->screenHeight;
}
