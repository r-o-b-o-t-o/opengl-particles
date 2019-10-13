#ifndef SPARK_H
#define SPARK_H

#include "Particle.h"

#define SPARK_MAX_LIFETIME 5.0f

class Spark : public Particle {
public:
	Spark();
	Spark(const glm::vec3 &position, float ttl);

	void initialize(float dt) override;
	void update(float deltaTime) override;

private:

};

#endif