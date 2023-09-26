//
// Created by edgar on 9/26/2023.
//
#include "ObjectReader.h"
#include <iostream>

int main()
{
	ObjectReader reader("ObjParsed/vertex.txt", "ObjParsed/faces.txt");
	reader.LoadAll();
	return 0;
}