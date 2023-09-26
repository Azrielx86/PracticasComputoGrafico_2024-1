#include "Mesh.h"

MeshColor::MeshColor()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	vertexCount = 0;
}

void MeshColor::CreateMeshColor(GLfloat *vertices, unsigned int numOfVertices)
{
	vertexCount = numOfVertices;
	glGenVertexArrays(1, &VAO); // generar 1 VAO
	glBindVertexArray(VAO);     // asignar VAO

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW); // pasarle los datos al VBO asignando tamaño, los datos y en este caso es estático pues no se modificarán los valores
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(vertices[0]), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(vertices[0]), (void *) (3 * sizeof(vertices[0]))); // Offset de los 3 vértices para tomar los 3 de color
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void MeshColor::CreateMeshColor(std::vector<GLfloat> &vertices, std::vector<unsigned int> &indices, unsigned int numOfVertices, unsigned int numberOfIndices)
{
	indexCount = numberOfIndices;
	glGenVertexArrays(1, &VAO); // generar 1 VAO
	glBindVertexArray(VAO);     // asignar VAO

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numberOfIndices, &indices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, &vertices[0], GL_STATIC_DRAW); // pasarle los datos al VBO asignando tamaño, los datos y en este caso es estático pues no se modificarán los valores

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(vertices[0]), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(vertices[0]), (void *) (3 * sizeof(vertices[0]))); // Offset de los 3 vértices para tomar los 3 de color
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void MeshColor::RenderMeshColor()
{
	////////////Para dibujar desde los vértices
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void MeshColor::ClearMeshColor()
{

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO); /// LIMPIAR BUFFER PARAE EVITAR OCUPAR LA MEMORIA
		VBO = 0;
	}
	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
	vertexCount = 0;
}

MeshColor::~MeshColor()
{
	ClearMeshColor();
}
