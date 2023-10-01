#pragma once
#include <glew.h>

class Texture
{
  public:
	Texture();
	Texture(const char *FileLoc);
	bool LoadTexture();
	bool LoadTextureA();
	void UseTexture();
	void ClearTexture();
	~Texture();
	static void DisableTextures();

  private:
	GLuint textureID;
	int width, height, bitDepth;
	const char *fileLocation;
};
