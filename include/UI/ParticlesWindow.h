#ifndef PARTICLES_WINDOW_H
#define PARTICLES_WINDOW_H

#include <vector>
#include <string>

class Application;

class ParticlesWindow {
public:
	ParticlesWindow(Application* app);
	void draw();

private:
	Application* app;

	std::vector<std::string> types;
	int selectedType;

	unsigned long maxParticlesValue;
	unsigned long minParticles;
	unsigned long maxParticles;

	float spawnRate;

};

#endif