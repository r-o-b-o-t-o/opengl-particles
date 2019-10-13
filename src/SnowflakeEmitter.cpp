#include "Random.h"
#include "SnowflakeEmitter.h"

SnowflakeEmitter::SnowflakeEmitter() :
		ParticleEmitter() {

	this->texture.loadFromFile("assets/img/snowflake.png");
}

void SnowflakeEmitter::pushParticle() {
	this->particles.push_back(new Snowflake());
}

void SnowflakeEmitter::initializeParticle(unsigned long idx) {
	Particle* p = this->particles[idx];
	p->initialize(10.0f);
	p->position = glm::vec3(Random::getFloat(-20.0f, 20.0f), 20.0f, Random::getFloat(-20.0f, 20.0f));
}
