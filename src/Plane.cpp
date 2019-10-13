#include <algorithm>
#include "glad/glad.h"
#include "glm/glm.hpp"

#include "Plane.h"

Plane::Plane() :
		show(true),
		shader("shaders/plane.vs.glsl", "shaders/plane.fs.glsl", "shaders/plane.gs.glsl") {

	this->vertices.push_back(0.0f);
	this->vertices.push_back(0.0f);
	this->vertices.push_back(0.0f);

	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), this->vertices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	this->shader.setMat4("model", glm::mat4x4(1.0f));
	this->setSize(10);
}

Plane::~Plane() {
	glDeleteVertexArrays(1, &this->vao);
	glDeleteBuffers(1, &this->vbo);
}

void Plane::draw() {
	if (!this->show) {
		return;
	}

	this->shader.use();

	glBindVertexArray(this->vao);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}

void Plane::setSize(int size) {
	size = glm::clamp(size, 1, PLANE_MAX_SIZE);
	this->shader.setInt("size", size);
}

Shader &Plane::getShader() {
	return this->shader;
}
