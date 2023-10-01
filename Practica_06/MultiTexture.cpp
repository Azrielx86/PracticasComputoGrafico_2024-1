//
// Created by edgar on 10/1/2023.
//

#include "MultiTexture.h"
bool MultiTexture::LoadTexture(bool alpha)
{
	stbi_convert_iphone_png_to_rgb(true);
	for (int i = 0; i < this->textures.size(); ++i)
	{
		auto texture = textures.at(i);
		// xd
		unsigned char *texData = stbi_load(textures.at(i).fileLocation, &textures.at(i).width, &textures.at(i).height, &textures.at(i).bitDepth, STBI_rgb_alpha);
		if (!texData)
			printf("Error cargando la textura: %s", texture.fileLocation);
	}
	return true;
}

MultiTexture::MultiTexture(const std::vector<std::string> &files)
{
	for (const std::string &file : files)
	{
		MultiTexture::TextureFile textureFile{};
		textureFile.fileLocation = file.c_str();
		textureFile.bitDepth = 0;
		textureFile.height = 0;
		textureFile.width = 0;
		this->textures.push_back(textureFile);
	}
}
