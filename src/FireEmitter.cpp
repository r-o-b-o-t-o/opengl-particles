#include "Random.h"
#include "FireEmitter.h"
#include "Spark.h"

FireEmitter::FireEmitter() :
		ParticleEmitter() {

	this->texture.loadFromFile("assets/img/confetti.png");
}

void FireEmitter::pushParticle() {
	this->particles.push_back(new Spark());
}

void FireEmitter::initializeParticle(unsigned long idx) {
	Particle* p = this->particles[idx];
	p->initialize(SPARK_MAX_LIFETIME);
}
