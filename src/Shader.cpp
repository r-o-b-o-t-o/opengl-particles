#include <iostream>

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

#include "shader.h"

std::map<std::string, unsigned int> Shader::compiledShaders;

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
	unsigned int vertex = this->getShader(vertexPath, GL_VERTEX_SHADER);
	unsigned int fragment = this->getShader(fragmentPath, GL_FRAGMENT_SHADER);
	unsigned int geometry;
	if (geometryPath != nullptr) {
		geometry = this->getShader(geometryPath, GL_GEOMETRY_SHADER);
	}

	int success;
	char infoLog[512];

	this->id = glCreateProgram();
	glAttachShader(this->id, vertex);
	glAttachShader(this->id, fragment);
	if (geometryPath != nullptr) {
		glAttachShader(this->id, geometry);
	}
	glLinkProgram(this->id);
	glGetProgramiv(this->id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->id, 512, nullptr, infoLog);
		std::cerr << "Shader program linking failed: " << infoLog << std::endl;
	}
}

std::string Shader::readShaderFile(const char* path) const {
	std::string code;
	std::ifstream file;

	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		file.open(path);

		std::stringstream stream;
		stream << file.rdbuf();
		file.close();

		code = stream.str();
	}
	catch (std::ifstream::failure e) {
		std::cerr << "Could not read shader file \"" << path << "\": " << e.what() << std::endl;
	}

	return code;
}

void Shader::use() const {
	glUseProgram(this->id);
}

unsigned int Shader::getId() const {
	return this->id;
}

int Shader::getUniformLocation(const std::string &name) const {
	return glGetUniformLocation(this->id, name.c_str());
}

void Shader::setBool(const std::string &name, bool value) const {
	int location = this->getUniformLocation(name);
	this->use();
	glUniform1i(location, static_cast<int>(value));
}

void Shader::setInt(const std::string &name, int value) const {
	int location = this->getUniformLocation(name);
	this->use();
	glUniform1i(location, value);
}

void Shader::setFloat(const std::string &name, float value) const {
	int location = this->getUniformLocation(name);
	this->use();
	glUniform1f(location, value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &vec) const {
	this->setVec2(name, vec.x, vec.y);
}

void Shader::setVec2(const std::string &name, float x, float y) const {
	int location = this->getUniformLocation(name);
	this->use();
	glUniform2f(location, x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &vec) const {
	this->setVec3(name, vec.x, vec.y, vec.z);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const {
	int location = this->getUniformLocation(name);
	this->use();
	glUniform3f(location, x, y, z);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
	int location = this->getUniformLocation(name);
	this->use();
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

unsigned int Shader::getShader(const char* path, unsigned int shaderType) {
	std::string strPath = std::string(path);
	if (compiledShaders.count(strPath) > 0) {
		return compiledShaders[strPath];
	}

	std::string code = this->readShaderFile(path);
	const char* codeC = code.c_str();

	unsigned int shader;
	int success;
	char infoLog[512];

	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &codeC, nullptr);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cerr << "Shader compilation failed: " << infoLog << std::endl;
	}
	else {
		compiledShaders[strPath] = shader;
	}

	return shader;
}

void Shader::deleteShaders() {
	for (auto &p : compiledShaders) {
		glDeleteShader(p.second);
	}
}
