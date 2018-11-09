#pragma once
#include "math_3d.h"
#include <vector>
using std::vector;

struct Vertex
{
	Vector3f m_pos;
	//Vector2f m_tex;

	Vertex() {}

	/*Vertex(Vector3f pos, Vector2f tex)
	{
		m_pos = pos;
		m_tex = tex;
	}*/
	Vertex(Vector3f pos)
	{
		m_pos = pos;
	}
};

class ObjLoader
{
private:
	vector<Vertex> VertexBuffer;
	vector<unsigned int> IndexBuffer;
public:
	void load(const char* filename);
	vector<Vertex> getVertexBuffer();
	vector<unsigned int> getIndexBuffer();
};
