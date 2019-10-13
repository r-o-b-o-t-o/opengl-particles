#include <algorithm>
#include "glad/glad.h"

#include "Random.h"
#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter() :
		lastUsedParticle(0),
		instanceStride(9),
		maxParticles(N_MAX_PARTICLES / 10ul),
		spawnRate(static_cast<float>(maxParticles) / 10.0f),
		shader("shaders/particle.vs.glsl", "shaders/particle.fs.glsl") {

	this->texture.loadFromFile("assets/img/confetti.png");
}

void ParticleEmitter::initialize() {
	this->particles.reserve(N_MAX_PARTICLES);
	for (unsigned long i = 0; i < N_MAX_PARTICLES; ++i) {
		this->pushParticle();
	}
	this->instanceData.reserve(N_MAX_PARTICLES * this->instanceStride);
	for (unsigned long i = 0; i < N_MAX_PARTICLES * this->instanceStride; ++i) {
		this->instanceData.emplace_back();
	}

	float vertices[] = {
		// positions          // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f,         // top right
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,         // bottom right
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f,         // top left

		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,         // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,         // bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f,         // top left
	};
	for (int i = 0; i < sizeof(vertices) / sizeof(vertices[0]); ++i) {
		this->data[i] = vertices[i];
	}

	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &this->vbo);
	glGenBuffers(1, &this->instanceVbo);

	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->data), this->data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // aPos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1); // aTexCoords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, this->instanceVbo);
	glBufferData(GL_ARRAY_BUFFER, this->instanceData.size() * sizeof(float), this->instanceData.data(), GL_STREAM_DRAW);

	glEnableVertexAttribArray(2); // aInstancePos
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, this->instanceStride * sizeof(float), (void*)0);
	glVertexAttribDivisor(2, 1);

	glEnableVertexAttribArray(3); // aInstanceColor
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, this->instanceStride * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribDivisor(3, 1);

	glEnableVertexAttribArray(4); // aInstanceScale
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, this->instanceStride * sizeof(float), (void*)(7 * sizeof(float)));
	glVertexAttribDivisor(4, 1);
}

ParticleEmitter::~ParticleEmitter() {
	glDeleteVertexArrays(1, &this->vao);
	glDeleteBuffers(1, &this->vbo);
	glDeleteBuffers(1, &this->instanceVbo);

	for (Particle* p : this->particles) {
		delete p;
	}
}

void ParticleEmitter::generateParticles(float dt) {
	int n = static_cast<unsigned long>(dt * this->spawnRate);
	unsigned long idx;
	for (unsigned long i = 0; i < n; ++i) {
		if (this->findUnusedIndex(&idx)) {
			this->initializeParticle(idx);
		}
	}
}

void ParticleEmitter::update(float dt) {
	this->update(dt, 0, this->maxParticles);
}

void ParticleEmitter::update(float dt, unsigned long offset, unsigned long count) {
	unsigned long last = offset + count;
	Particle* p;
	for (unsigned long i = offset; i < last; ++i) {
		p = this->particles[i];

		if (p->isAlive()) {
			p->update(dt);

			this->instanceData[this->instanceStride * i] = p->position.x;
			this->instanceData[this->instanceStride * i + 1] = p->position.y;
			this->instanceData[this->instanceStride * i + 2] = p->position.z;

			this->instanceData[this->instanceStride * i + 3] = p->color.r;
			this->instanceData[this->instanceStride * i + 4] = p->color.g;
			this->instanceData[this->instanceStride * i + 5] = p->color.b;
			this->instanceData[this->instanceStride * i + 6] = p->color.a;

			this->instanceData[this->instanceStride * i + 7] = p->scale.x;
			this->instanceData[this->instanceStride * i + 8] = p->scale.y;
		} else {
			this->instanceData[this->instanceStride * i + 6] = 0.0f;
		}
	}
}

void ParticleEmitter::pushParticle() {
	this->particles.push_back(new Particle());
}

void ParticleEmitter::initializeParticle(unsigned long idx) {
	Particle* p = this->particles[idx];
	p->initialize(5.0f);
	p->position = glm::vec3(0.0f);
	p->scale = glm::vec3(1.0f);
	p->color = glm::vec4(Random::getFloat(0.0f, 1.0f), Random::getFloat(0.0f, 1.0f), Random::getFloat(0.0f, 1.0f), 1.0f);
	p->speed = glm::vec3(Random::getFloat(-20.0f, 20.0f), Random::getFloat(20.0f, 50.0f), Random::getFloat(-20.0f, 20.0f));
}

void ParticleEmitter::draw() {
	this->copyDataToBuffer();

	this->texture.bind();
	this->shader.use();
	glBindVertexArray(this->vao);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, this->maxParticles);
	glBindVertexArray(0);
}

Shader &ParticleEmitter::getShader() {
	return this->shader;
}

bool ParticleEmitter::findUnusedIndex(unsigned long* idx) {
	for (unsigned long i = this->lastUsedParticle; i < this->maxParticles; i++) {
		if (!this->particles[i]->isAlive()) {
			this->lastUsedParticle = i;
			*idx = i;
			return true;
		}
	}

	for (unsigned long i = 0; i < this->lastUsedParticle; i++) {
		if (!this->particles[i]->isAlive()) {
			this->lastUsedParticle = i;
			*idx = i;
			return true;
		}
	}

	return false;
}

void ParticleEmitter::copyDataToBuffer() {
	glBindBuffer(GL_ARRAY_BUFFER, this->instanceVbo);
	glBufferData(GL_ARRAY_BUFFER, this->instanceData.size() * sizeof(float), NULL, GL_STREAM_DRAW); // Buffer orphaning
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->maxParticles * this->instanceStride * sizeof(float), this->instanceData.data());
}

void ParticleEmitter::setMaxParticles(unsigned long n) {
	this->maxParticles = glm::clamp(n, 0ul, N_MAX_PARTICLES);
}

unsigned long ParticleEmitter::getMaxParticles() const {
	return this->maxParticles;
}

void ParticleEmitter::setSpawnRate(float rate) {
	this->spawnRate = glm::clamp(rate, 0.0f, static_cast<float>(this->maxParticles));
}

float ParticleEmitter::getSpawnRate() const {
	return this->spawnRate;
}
