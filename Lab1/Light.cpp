#include "Light.h"

/**
 * 计算对应顶点数组中的每一个顶点的顶点法向量(在不是从外部读入法向量的情况下)
 * pIndices：索引数组
 * IndexCount：索引数量
 * pVertices：顶点数组
 * VertexCount：顶点数量
**/
void Light::CalcNormals(vector<unsigned int>* Indices, vector<Vertex>* Vertices)
{
	vector<unsigned int> pIndices = *Indices;
	vector<Vertex> pVertices = *Vertices;

	for (unsigned int i = 0; i < pIndices.size(); i += 3) {
		unsigned int Index0 = pIndices[i];
		unsigned int Index1 = pIndices[i + 1];
		unsigned int Index2 = pIndices[i + 2];
		Vector3f v1 = pVertices[Index1].m_pos - pVertices[Index0].m_pos;
		Vector3f v2 = pVertices[Index2].m_pos - pVertices[Index0].m_pos;
		Vector3f Normal = v1.Cross(v2);
		Normal.Normalize();

		pVertices[Index0].m_nor += Normal;
		pVertices[Index1].m_nor += Normal;
		pVertices[Index2].m_nor += Normal;
	}

	for (unsigned int i = 0; i < pVertices.size(); i++) {
		pVertices[i].m_nor.Normalize();
	}
}

void Light::CalcNormals_Obj(vector<Vertex>* Vertices)
{
	vector<Vertex> pVertices = *Vertices;

	for (unsigned int i = 0; i < pVertices.size(); i++) {
		pVertices[i].m_nor.Normalize();
	}
}

