//
// Created by edgar on 9/26/2023.
//

#include "ObjectReader.h"

#include <utility>

ObjectReader::ObjectReader(std::string vertexPath, std::string facesPath) : vertexPath(std::move(vertexPath)), facesPath(std::move(facesPath)) {}

void ObjectReader::LoadVertex()
{
	std::ifstream inStream(this->vertexPath);
	std::string line;
	std::stringstream floatStream;

	while (std::getline(inStream, line))
	{
		floatStream = std::stringstream(line);
		float value;
		while (floatStream >> value)
			this->_vertex.push_back(value);
	}
}

void ObjectReader::LoadFaces()
{
	std::ifstream inStream(this->facesPath);
	std::string line;
	std::stringstream intStream;

	while (std::getline(inStream, line))
	{
		intStream = std::stringstream(line);
		int value;
		while (intStream >> value)
			this->_faces.push_back(value);
	}
}

void ObjectReader::LoadAll()
{
	LoadVertex();
	LoadFaces();
}
const std::vector<float> &ObjectReader::getVertex() const
{
	return _vertex;
}

std::vector<unsigned int> ObjectReader::getFaces() const
{
	return _faces;
}
