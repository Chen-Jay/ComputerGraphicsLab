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
			token = line.substr(0,line.find(" "));
			line = line.substr(line.find(" ")+1, line.length());
			if (token == "v")
			{				
				float x;
				float y;
				float z;
				for(int i=0;i<3;i++)
				{
					token = line.substr(0,line.find(" "));
					stringstream ss;
					ss << token;
					line = line.substr(line.find(" ")+1, line.length());
					if (i == 0)
					{
						ss >> x;
						x /= 30.0;
					}
					else if (i == 1)
					{
						ss >> y;
						y /=30.0;
					}
					else if (i == 2)
					{
						ss >> z;
						z /= 30.0;
					}
				}
				Vector3f vector(x, y, z);
				VertexBuffer.push_back(Vertex(vector));
			}
			else if (token == "f")
			{
				unsigned int x;
				unsigned int y;
				unsigned int z;
				for (int i = 0; i < 3; i++)
				{
					token = line.substr(0,line.find(" "));
					stringstream ss;
					ss << token;
					line = line.substr(line.find(" ")+1, line.length());
					if (i == 0)
					{
						ss >> x;
						x -= 1;
					}
					else if (i == 1)
					{
						ss >> y;
						y -= 1;
					}
					else if (i == 2)
					{
						ss >> z;
						z -= 1;
					}
				}
				IndexBuffer.push_back(x);
				IndexBuffer.push_back(y);
				IndexBuffer.push_back(z);
			}
			
		}

		f.close();
	/*	for (int i = 0; i < IndexBuffer.size(); i++)
		{
			std::cout << IndexBuffer[i] << " ";
		}
		for (int i = 0; i < VertexBuffer.size(); i++)
		{
			VertexBuffer[i].m_pos.Print();
		}*/
	}
}

vector<Vertex> ObjLoader::getVertexBuffer()
{
	return VertexBuffer;
}

vector<unsigned int> ObjLoader::getIndexBuffer()
{
	return IndexBuffer;
}
