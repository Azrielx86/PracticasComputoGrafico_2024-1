#pragma once

#include <fstream>
#include <glew.h>
#include <iostream>
#include <stdio.h>
#include <string>

class Shader
{
  public:
	Shader();

	void CreateFromString(const char *vertexCode, const char *fragmentCode);
	void CreateFromFiles(const char *vertexLocation, const char *fragmentLocation);

	std::string ReadFile(const char *fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetEyePositionLocation();
	GLuint getColorLocation();
	void UseShader();
	void ClearShader();

	~Shader();

  private:
	GLuint shaderID, uniformProjection, uniformModel, uniformColor, uniformView, uniformEyePosition;
	void CompileShader(const char *vertexCode, const char *fragmentCode);
	void AddShader(GLuint theProgram, const char *shaderCode, GLenum shaderType);
};
