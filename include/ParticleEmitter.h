#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#define N_MAX_PARTICLES 100'000ul

#include <mutex>
#include <vector>

#include "Shader.h"
#include "Texture.h"
#include "Particle.h"

class ParticleEmitter {
public:
	ParticleEmitter();
	~ParticleEmitter();

	ParticleEmitter(const ParticleEmitter&) = delete;
	ParticleEmitter& operator=(const ParticleEmitter&) = delete;

	void generateParticles(float dt);
	void update(float dt);
	void update(float dt, unsigned long offset, unsigned long count);
	void draw();
	void initialize();

	void setMaxParticles(unsigned long n);
	unsigned long getMaxParticles() const;

	void setSpawnRate(float rate);
	float getSpawnRate() const;

	Shader &getShader();

protected:
	Shader shader;
	Texture texture;

	unsigned int vao;
	unsigned int vbo;
	unsigned int instanceVbo;
	float data[5 * 6];
	unsigned long instanceStride;
	std::vector<float> instanceData;
	std::vector<Particle*> particles;

	unsigned long maxParticles;
	float spawnRate;
	std::mutex mutex;

	unsigned long lastUsedParticle;
	bool findUnusedIndex(unsigned long* idx);
	virtual void pushParticle();
	virtual void initializeParticle(unsigned long idx);
	void copyDataToBuffer();
};

#endif