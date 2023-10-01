//
// Created by edgar on 10/1/2023.
//

#ifndef PRACTICA06_MULTITEXTURE_H
#define PRACTICA06_MULTITEXTURE_H
#include "CommonValues.h"
#include "Texture.h"
#include <glew.h>
#include <vector>
#include <string>

// TODO : Para cargar múltiples texturas
class MultiTexture
{
	struct TextureFile
	{
		int width, height, bitDepth;
		const char *fileLocation;
	};

  public:
	explicit MultiTexture(const std::vector<std::string>& files);
	bool LoadTexture(bool alpha = true);
	void UseTexture();
	void ClearTexture();
	~MultiTexture();

  private:
	//	GLuint textureID;
	std::vector<TextureFile> textures;
	std::vector<GLuint> texturesID;
};

#endif // PRACTICA06_MULTITEXTURE_H
