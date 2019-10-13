#ifndef APPLICATION_H
#define APPLICATION_H

#include "GLFW/glfw3.h"

#include "OrbitCamera.h"
#include "Plane.h"
#include "UI/DebugWindow.h"
#include "UI/PlaneWindow.h"
#include "UI/ParticlesWindow.h"
#include "ParticleEmitter.h"
#include "SnowflakeEmitter.h"
#include "FireEmitter.h"

class Application {
public:
	Application(int screenWidth, int screenHeight);

	OrbitCamera camera;
	Plane plane;
	DebugWindow debugWindow;
	PlaneWindow planeWindow;
	ParticlesWindow particlesWindow;
	ParticleEmitter confettiEmitter;
	SnowflakeEmitter snowflakeEmitter;
	FireEmitter fireEmitter;
	ParticleEmitter* activeEmitter;

	void draw();
	void update(GLFWwindow* window, float deltaTime);
	void onWindowSizeChanged(int width, int height);
	void drawParticles();
	void onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	int getScreenWidth() const;
	int getScreenHeight() const;

private:
	void drawUi();
	void takeScreenshot() const;

	int screenWidth;
	int screenHeight;

};

#endif