#include <math.h>

#include "Snowflake.h"
#include "Random.h"

constexpr auto PI = 3.14159265f;

Snowflake::Snowflake() :
		Snowflake(glm::vec3(0.0f), 0.0f) {

}

Snowflake::Snowflake(const glm::vec3 &position, float ttl) {
	this->initialize(ttl);
	this->position = position;
	this->scale = glm::vec2(Random::getFloat(0.1f, 0.5f));
}

void Snowflake::initialize(float ttl) {
	Particle::initialize(ttl);

	this->rebound = false;

	this->acceleration.y = 0.0f;
	this->speed.y = Random::getFloat(-2.0f, -0.5f);
	this->swayAmplitude = Random::getFloat(0.5f, 2.0f);
	this->swaySpeed = Random::getFloat(0.5f, 2.0f);

	float angle = Random::getFloat(-PI, PI);
	this->swayDirection = glm::vec2(std::cos(angle), std::sin(angle));
	this->sway = glm::vec2(0.0f);
}

void Snowflake::update(float deltaTime) {
	Particle::update(deltaTime);

	this->position -= glm::vec3(this->sway.x, 0.0f, this->sway.y);
	this->sway = this->swayDirection * std::cos(this->getTimeToLive() * this->swaySpeed) * this->swayAmplitude;
	this->position += glm::vec3(this->sway.x, 0.0f, this->sway.y);
}
