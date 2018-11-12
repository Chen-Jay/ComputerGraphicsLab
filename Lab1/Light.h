#pragma once
#include "ObjLoader.h"
class Light
{
public:
	void CalcNormals(vector<unsigned int>* Indices, vector<Vertex>* Vertices);
	void CalcNormals_Obj(vector<Vertex>* Vertices);
};