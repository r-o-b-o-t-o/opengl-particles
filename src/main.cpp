#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "stb_image.h"
#include "stb_image_write.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "pch.h"
#include "Application.h"
#include "ThreadPool.h"
#include "ParticleEmitter.h"

void onError(int error, const char* description);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void scrollCallback(GLFWwindow* window, double offsetX, double offsetY);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(int argc, char** argv) {
	glfwSetErrorCallback(onError);
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4); // Multisampling
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int width = 800, height = 600;
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Particles", nullptr, nullptr);
	if (window == nullptr) {
		std::cerr << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetKeyCallback(window, keyCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD." << std::endl;
		return 1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::GetIO().IniFilename = nullptr;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Application app(width, height);
	ThreadPool pool(5);
	for (unsigned int i = 0; i < pool.getSize(); ++i) {
		if (i == 0) {
			// First thread only creates particles
			pool.execute([&app, &pool]() {
				double time, lastTime = 0, deltaTime;
				float dt;

				while (true) {
					time = glfwGetTime();
					deltaTime = time - lastTime;
					lastTime = time;
					dt = static_cast<float>(deltaTime);

					app.activeEmitter->generateParticles(dt);

					if (pool.shuttingDown()) {
						break;
					}

					std::this_thread::sleep_for(std::chrono::milliseconds(33));
				}
			});
		} else {
			// The other threads update the particles
			pool.execute([&app, &pool, i]() {
				double time, lastTime = 0, deltaTime;
				float dt;

				unsigned long count = N_MAX_PARTICLES / (pool.getSize() - 1);
				unsigned long offset = (i - 1) * count;

				while (true) {
					time = glfwGetTime();
					deltaTime = time - lastTime;
					lastTime = time;
					dt = static_cast<float>(deltaTime);

					app.activeEmitter->update(dt, offset, count);

					if (pool.shuttingDown()) {
						break;
					}

					std::this_thread::sleep_for(std::chrono::milliseconds(5));
				}
			});
		}
	}

	double currentTime, lastTime = 0, deltaTime, lastFramerateMeasure = 0;
	float deltaTimeFloat;
	int framesCounter = 0;

	glfwSetWindowUserPointer(window, &app);
	while (!glfwWindowShouldClose(window)) {
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		deltaTimeFloat = static_cast<float>(deltaTime);
		lastTime = currentTime;
		++framesCounter;

		if (currentTime - lastFramerateMeasure >= 0.1) {
			app.debugWindow.framerate = static_cast<int>(std::round(static_cast<double>(framesCounter) / (currentTime - lastFramerateMeasure)));
			framesCounter = 0;
			lastFramerateMeasure = currentTime;
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		bool imguiMouseFocus = ImGui::IsAnyWindowHovered() || ImGui::GetIO().WantCaptureMouse;
		if (!imguiMouseFocus) {
			app.update(window, deltaTimeFloat);
		}

		glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		app.draw();
		app.drawParticles();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	pool.shutdown();

	Shader::deleteShaders();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void onError(int error, const char* description) {
	std::cerr << "Error " << error << ": " << description << std::endl;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	app->onWindowSizeChanged(width, height);
}

void scrollCallback(GLFWwindow* window, double offsetX, double offsetY) {
	if (ImGui::IsAnyWindowHovered() || ImGui::GetIO().WantCaptureMouse) {
		return;
	}

	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	app->camera.zoom(static_cast<float>(offsetY));
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (ImGui::IsAnyWindowHovered() || ImGui::GetIO().WantCaptureMouse) {
		return;
	}

	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	app->onKeyEvent(window, key, scancode, action, mods);
}
