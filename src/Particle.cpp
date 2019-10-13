#include "glad/glad.h"
#include "glm/gtx/transform.hpp"
#include <math.h>
#include <algorithm>

#include "Particle.h"

Particle::Particle() :
		Particle(glm::vec3(), glm::vec2(), 0.0f) {

}

Particle::Particle(const glm::vec3 &position, const glm::vec2 &scale, float ttl) :
		scale(scale),
		position(position),
		acceleration(0.0f, -18.0f, 0.0f) {

	this->initialize(ttl);
}

void Particle::initialize(float ttl) {
	this->speed = glm::vec3(0.0f);
	this->color = glm::vec4(1.0f);
	this->rebound = true;
	this->timeToLive = ttl;
}

void Particle::update(float deltaTime) {
	this->timeToLive -= deltaTime;

	if (!this->isAlive()) {
		return;
	}

	this->speed += this->acceleration * deltaTime;
	this->position += this->speed * deltaTime;

	if (this->rebound) {
		const float e = 0.4f;
		if (this->position.y <= 0.0f && this->speed.y < 0.0f) {
			this->speed *= e;
			this->speed.y *= -1.0f;
		}
		this->position.y = std::max(this->position.y, 0.0f);
	}
}

void Particle::translate(const glm::vec3 &delta) {
	this->position += delta;
}

bool Particle::isAlive() const {
	return this->timeToLive > 0.0f;
}

float Particle::getTimeToLive() const {
	return this->timeToLive;
}
