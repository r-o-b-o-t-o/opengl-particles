#include "glm/gtx/compatibility.hpp"
#include <math.h>

#include "Spark.h"
#include "Random.h"

constexpr auto PI = 3.14159265f;

Spark::Spark() :
		Spark(glm::vec3(0.0f), 0.0f) {

}

Spark::Spark(const glm::vec3 &position, float ttl) {
	this->initialize(ttl);
	this->position = position;
	this->scale = glm::vec2(Random::getFloat(0.1f, 0.5f));
}

void Spark::initialize(float ttl) {
	Particle::initialize(ttl);
	this->rebound = false;

	float angle = Random::getFloat(-PI, PI);
	glm::vec2 amplitude = glm::vec2(std::cos(angle), std::sin(angle)) * Random::getFloat(0.1f, 2.0f);
	this->speed = glm::vec3(amplitude.x, Random::getFloat(1.0f, 2.0f), amplitude.y);
	this->acceleration = glm::vec3(0.0f);
	this->position = glm::vec3(0.0f, 0.5f, 0.0f);
}

void Spark::update(float deltaTime) {
	Particle::update(deltaTime);

	float t = 1.0f - (this->getTimeToLive() / SPARK_MAX_LIFETIME);

	// Color over lifetime
	this->color = glm::lerp(glm::vec4(1.0f, 0.83f, 0.2f, 1.0f), glm::vec4(0.85f, 0.0f, 0.1f, 1.0f), t);

	// Size over lifetime
	this->scale = glm::vec2(glm::lerp(1.0f, 0.2f, t));

	// Movement
	this->position.x = std::sin(t * PI) * this->speed.x;
	this->position.z = std::sin(t * PI) * this->speed.z;
}
