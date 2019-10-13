#ifndef TEXTURE_H
#define TEXTURE_H

#include <map>
#include <string>

class Texture {
public:
	Texture();
	Texture(const std::string &path);
	~Texture();

	Texture(const Texture&) = delete;
	Texture &operator=(const Texture&) = delete;

	int getWidth() const;
	int getHeight() const;
	int getChannels() const;
	void bind() const;
	void loadFromFile(const std::string &path);

private:
	int width;
	int height;
	int channels;
	unsigned int texture;

	static std::map<std::string, Texture*> loadedTextures;
};

#endif