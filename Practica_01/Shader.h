#pragma once
#include<stdio.h> // NOLINT(*-deprecated-headers)
#include<string>
#include<iostream>
#include<fstream>

#ifdef CMAKE_PROJECT
#include <GL/glew.h>
#else
#include<glew.h>
#endif

class Shader
{
public:
	Shader();
	void CreatefromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	std::string ReadFile(const char*  fileLocation);
	GLuint getProjectLocation();
	GLuint getModelLoaction();

	void useShader();
	void ClearShader();
	~Shader();

private: 
	GLuint shaderID, uniformProjection, uniformModel;
	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* ShaderCode, GLenum shaderType);
};

