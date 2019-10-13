#ifndef FIRE_EMITTER_H
#define FIRE_EMITTER_H

#include "ParticleEmitter.h"

class FireEmitter : public ParticleEmitter {
public:
	FireEmitter();

protected:
	void pushParticle() override;
	void initializeParticle(unsigned long idx) override;
};

#endif