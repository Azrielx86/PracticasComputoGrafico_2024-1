#pragma once
#ifdef CMAKE_PROJECT
#include <GL/glew.h>
#else
#include<glew.h>
#endif
class Mesh
{
public:
	Mesh();
	void CreateMesh(GLfloat *vertices,unsigned int *indices, unsigned int numOfVertices, unsigned int numberOfIndices);
	void RenderMesh();
	void ClearMesh();
	~Mesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;

};

