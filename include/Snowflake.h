#ifndef SNOWFLAKE_H
#define SNOWFLAKE_H

#include "Particle.h"

class Snowflake : public Particle {
public:
	Snowflake();
	Snowflake(const glm::vec3 &position, float ttl);

	void initialize(float dt) override;
	void update(float deltaTime) override;

private:
	float swayAmplitude;
	float swaySpeed;
	glm::vec2 swayDirection;
	glm::vec2 sway;
};

#endif