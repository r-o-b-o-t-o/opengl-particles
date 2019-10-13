#ifndef PARTICLE_H
#define PARTICLE_H

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

class Particle {
public:
	Particle();
	Particle(const glm::vec3 &position, const glm::vec2 &scale, float ttl);

	virtual void initialize(float ttl);
	virtual void update(float deltaTime);
	void translate(const glm::vec3 &delta);
	float getTimeToLive() const;
	bool isAlive() const;
	bool rebound;

	glm::vec3 position;
	glm::vec2 scale;
	glm::vec3 speed;
	glm::vec3 acceleration;
	glm::vec4 color;

protected:
	float timeToLive;
};

#endif