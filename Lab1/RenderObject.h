#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string> 
using std::string;
#include "ObjLoader.h"
#include "texture.h"
#include "camera.h"
#include "transform.h"
class RenderObject 
{
public :
	/**物体本身数据**/
	GLuint VBO; //用来存储物体顶点数据的缓冲对象(在GPU中）

	ObjLoader objLoader; //obj文件读取及数据保存

	GLuint gSampler; //纹理取样器句柄

	Texture* Texture; //Texture类的对象（内含 纹理对象、纹理目标、默认纹理单元）

	Transform objectTransformController;

	/**shader相关**/
	//shader文件名
	string pVSFileName; //vertex shader文件名
	string pFSFileName; //fragment shader文件名

	//shader使用数据
	GLuint gWVPLocation; //变换至眼睛坐标系的变换矩阵
	GLuint gWorldLocation; //变换至世界坐标系的变换矩阵
	GLuint gEyePosition; //眼睛所处于的坐标

	Camera* camera;
	//使用的shader程序句柄
	GLuint ShaderProgram;

	RenderObject(const char* vshader, const char* fshader);
	
	void render();

};