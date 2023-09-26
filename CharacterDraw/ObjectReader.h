//
// Created by edgar on 9/26/2023.
//

#ifndef MAIN_OBJECTREADER_H
#define MAIN_OBJECTREADER_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class ObjectReader
{
  public:
	ObjectReader(std::string vertexPath, std::string facesPath);
	void LoadAll();
	void LoadVertex();
	void LoadFaces();
	[[nodiscard]] const std::vector<float> &getVertex() const;
	[[nodiscard]] std::vector<unsigned int> getFaces() const;

  private:
	std::vector<float> _vertex;
	std::vector<unsigned int> _faces;
	std::string vertexPath;
	std::string facesPath;
};

#endif // MAIN_OBJECTREADER_H
