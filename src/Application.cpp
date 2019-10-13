#include <time.h>
#include "Application.h"
#include "stb_image_write.h"

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

void Application::onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (key == GLFW_KEY_PRINT_SCREEN && action == GLFW_PRESS) {
		this->takeScreenshot();
	}
}

void Application::takeScreenshot() const {
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	int x = viewport[0];
	int y = viewport[1];
	int width = viewport[2];
	int height = viewport[3];

	char* data = new char[width * height * 3];
	if (data != nullptr) {
		time_t now = time(nullptr);
		tm localTime;
		char fileName[40];
		localtime_s(&localTime, &now);
		strftime(fileName, 40, "screenshots/%Y%m%d_%H%M%S.png", &localTime);

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

		stbi_flip_vertically_on_write(1);
		int saved = stbi_write_png(fileName, width, height, 3, data, 0);

		delete[] data;
	}
}
