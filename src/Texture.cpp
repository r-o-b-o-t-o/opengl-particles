#include "stb_image.h"
#include "glad/glad.h"
#include <iostream>

#include "Texture.h"

std::map<std::string, Texture*> Texture::loadedTextures;

Texture::Texture() :
		texture(0),
		channels(0),
		width(0),
		height(0) {

}

Texture::Texture(const std::string &path) {
	this->loadFromFile(path);
}

void Texture::loadFromFile(const std::string &path) {
	if (loadedTextures.count(path) > 0) {
		this->texture = loadedTextures[path]->texture;
		this->width = loadedTextures[path]->width;
		this->height = loadedTextures[path]->height;
		this->channels = loadedTextures[path]->channels;
		return;
	}

	glGenTextures(1, &this->texture);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* data = stbi_load(path.c_str(), &this->width, &this->height, &this->channels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
		loadedTextures[path] = this;
	} else {
		std::cerr << "Could not load texture " << path << std::endl;
	}
}

Texture::~Texture() {
	glDeleteTextures(1, &this->texture);
}

void Texture::bind() const {
	if (this->texture != 0) {
		glBindTexture(GL_TEXTURE_2D, this->texture);
	}
}

int Texture::getWidth() const {
	return this->width;
}

int Texture::getHeight() const {
	return this->height;
}

int Texture::getChannels() const {
	return this->channels;
}
