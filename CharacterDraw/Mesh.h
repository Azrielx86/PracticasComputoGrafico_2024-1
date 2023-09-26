#pragma once
#ifdef CMAKE_PROJECT
#include <GL/glew.h>
#else
#include <glew.h>
#endif
#include <vector>
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
class MeshColor
{
public:
	MeshColor();
	void CreateMeshColor(GLfloat *vertices, unsigned int numOfVertices);
	void CreateMeshColor(std::vector<GLfloat>& vertices, std::vector<unsigned int>& indices, unsigned int numOfVertices, unsigned int numberOfIndices);
	void RenderMeshColor();
	void ClearMeshColor();
	~MeshColor();
private:
	GLuint VAO, VBO, IBO;
	GLsizei  vertexCount;
	GLsizei indexCount;
};

