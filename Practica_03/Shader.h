#pragma once
#include<stdio.h>
#include<string>
#include<iostream>
#include<fstream>

#include <stdio.h>
#ifdef CMAKE_PROJECT
#include <GL/glew.h>
#else
#include <glew.h>
#endif

class Shader
{
public:
	Shader();
	void CreatefromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	std::string ReadFile(const char*  fileLocation);
	GLuint getProjectLocation();
	GLuint getModelLocation();
	GLuint getViewLocation();
	GLuint getColorLocation();

	void useShader();
	void ClearShader();
	~Shader();

private: 
	GLuint shaderID, uniformProjection, uniformModel,uniformColor,uniformView;
	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* ShaderCode, GLenum shaderType);
};

