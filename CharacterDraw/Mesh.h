#pragma once
#include <glew.h>
#include <vector>

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

