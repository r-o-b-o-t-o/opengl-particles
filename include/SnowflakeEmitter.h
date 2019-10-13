#ifndef SNOWFLAKE_EMITTER_H
#define SNOWFLAKE_EMITTER_H

#include "Snowflake.h"
#include "ParticleEmitter.h"

class SnowflakeEmitter : public ParticleEmitter {
public:
	SnowflakeEmitter();

protected:
	void pushParticle() override;
	void initializeParticle(unsigned long idx) override;
};

#endif