#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

texture_t texture_new(const char *path, int flip, GLint type)
{
	int id = 0;

	int width = 0;
	int height = 0;
	int nrChannels = 0;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	stbi_set_flip_vertically_on_load(flip);
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	return id;
}

void texture_use(const texture_t t)
{
	glBindTexture(GL_TEXTURE_2D, t);
}