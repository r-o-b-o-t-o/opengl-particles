#ifndef PLANE_H
#define PLANE_H

#define PLANE_MAX_SIZE 25

#include <vector>

#include "Shader.h"

class Plane {
public:
	Plane();
	~Plane();

	void draw();
	void setSize(int size);
	Shader &getShader();

	bool show;

private:
	Shader shader;

	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	unsigned int vao;
	unsigned int vbo;

};

#endif