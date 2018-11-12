#include "ObjLoader.h"

#include <iostream>

#include <fstream>
using std::ifstream;
#include <string>
using std::string;
#include<sstream>
using std::stringstream;

void ObjLoader::load(const char * filename)
{
	ifstream f(filename, ifstream::in);
	if (f.is_open())
	{
		string line;
		string token;
		while (getline(f, line))
		{
			if (line.find(" ") == string::npos)
			{
				continue;
			}
			token = line.substr(0, line.find(" "));
			line = line.substr(line.find(" ") + 1, line.length());
			if (token == "v")	//顶点坐标读取
			{
				float x;
				float y;
				float z;
				for (int i = 0; i < 3; i++)
				{
					token = line.substr(0, line.find(" "));
					stringstream ss;
					ss << token;
					line = line.substr(line.find(" ") + 1, line.length());
					if (i == 0)
					{
						ss >> x;
					}
					else if (i == 1)
					{
						ss >> y;
					}
					else if (i == 2)
					{
						ss >> z;
					}
				}
				Vector3f vector(x, y, z);
				VertexBuffer.push_back(Vertex(vector));
			}
			else if (token == "vn") //法线数据
			{
				float x;
				float y;
				float z;
				for (int i = 0; i < 3; i++)
				{
					token = line.substr(0, line.find(" "));
					stringstream ss;
					ss << token;
					line = line.substr(line.find(" ") + 1, line.length());
					if (i == 0)
					{
						ss >> x;
					}
					else if (i == 1)
					{
						ss >> y;
					}
					else if (i == 2)
					{
						ss >> z;
					}
				}
				Vector3f normal(x, y, z);
				NormalBuffer.push_back(normal);
			}
			else if (token == "f")
			{
				unsigned int x_vertex; //顶点x
				unsigned int y_vertex; //顶点y
				unsigned int z_vertex; //顶点z

				unsigned int x_normal; //顶点x的法向量索引
				unsigned int y_normal; //顶点y的法向量索引
				unsigned int z_normal; //顶点z的法向量索引

				for (int i = 0; i < 3; i++)
				{
					token = line.substr(0, line.find(" "));

					line = line.substr(line.find(" ") + 1, line.length());
					if (i == 0)
					{
						string data = token.substr(0, token.find('/'));
						token = token.substr(token.find('/') + 2, token.length());
						stringstream ss1;
						ss1 << data;
						ss1 >> x_vertex;
						x_vertex -= 1;

						data = token.substr(0, token.length());
						stringstream ss2;
						ss2 << data;
						ss2 >> x_normal;
						x_normal -= 1;

						AllVertexBuffer.push_back(Vertex(VertexBuffer[x_vertex].m_pos, NormalBuffer[x_normal]));
					}
					else if (i == 1)
					{
						string data = token.substr(0, token.find('/'));
						token = token.substr(token.find('/') + 2, token.length());
						stringstream ss1;
						ss1 << data;
						ss1 >> y_vertex;
						y_vertex -= 1;

						data = token.substr(0, token.length());
						stringstream ss2;
						ss2 << data;
						ss2 >> y_normal;
						y_normal -= 1;

						AllVertexBuffer.push_back(Vertex(VertexBuffer[y_vertex].m_pos, NormalBuffer[y_normal]));
					}
					else if (i == 2)
					{
						string data = token.substr(0, token.find('/'));
						token = token.substr(token.find('/') + 2, token.length());
						stringstream ss1;
						ss1 << data;
						ss1 >> z_vertex;
						z_vertex -= 1;

						data = token.substr(0, token.length());
						stringstream ss2;
						ss2 << data;
						ss2 >> z_normal;
						z_normal -= 1;

						AllVertexBuffer.push_back(Vertex(VertexBuffer[z_vertex].m_pos, NormalBuffer[z_normal]));
					}
				}
			}
			else
			{
				continue;
			}
		}

		f.close();
		//std::cout << "hello";
		//for (int i = 0; i < VertexIndexBuffer.size(); i++)
		//{
		//	std::cout << VertexIndexBuffer[i] << " ";
		//}
		//std::cout << "\n\n";
		//for (int i = 0; i < VertexBuffer.size(); i++)
		//{
		//	VertexBuffer[i].m_pos.Print();
		//}
	}
}

vector<Vertex> ObjLoader::getVertexBuffer()
{
	return VertexBuffer;
}

vector<unsigned int> ObjLoader::getIndexBuffer()
{
	return VertexIndexBuffer;
}

vector<Vertex> ObjLoader::getAllVertexBuffer()
{
	return  AllVertexBuffer;
}


