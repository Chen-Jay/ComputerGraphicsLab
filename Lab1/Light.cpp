#include "Light.h"

/**
 * �����Ӧ���������е�ÿһ������Ķ��㷨����(�ڲ��Ǵ��ⲿ���뷨�����������)
 * pIndices����������
 * IndexCount����������
 * pVertices����������
 * VertexCount����������
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

