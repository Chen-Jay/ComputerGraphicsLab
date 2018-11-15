#pragma once
#include "math_3d.h"
#include <vector>
using std::vector;

struct Vertex
{
	Vector3f m_pos;	
	Vector2f m_tex;
	Vector3f m_nor;
	

	inline Vertex()
	{
		m_pos = Vector3f(0, 0, 0);
		m_tex = Vector2f(0, 0);
		m_nor = Vector3f(0, 0, 0);
	}

	Vertex(Vector3f pos)
	{
		m_pos = pos;
		m_nor = Vector3f(0.0,0.0,0.0);
		m_tex = Vector2f(0.0, 0.0);
	}

	Vertex(Vector3f pos, Vector3f nor)
	{
		m_pos = pos;
		m_nor = nor;
		m_tex = Vector2f(0.0,0.0);
	}

	Vertex(Vector3f pos, Vector3f nor,Vector2f tex)
	{
		m_pos = pos;
		m_nor = nor;
		m_tex = tex;
	}
};


class ObjLoader
{
private:
	vector<Vertex> VertexBuffer; //顶点
	vector<unsigned int> VertexIndexBuffer; //顶点索引
	vector<Vector3f> NormalBuffer; //法向量
	vector<unsigned int> NormalIndexBuffer; //法向量索引
	vector<Vector2f> TextureBuffer; //纹理坐标

	vector<Vertex> AllVertexBuffer;
public:
	void load(const char* filename);
	vector<Vertex> getVertexBuffer();
	vector<unsigned int> getIndexBuffer();

	vector<Vertex> getAllVertexBuffer();
};
