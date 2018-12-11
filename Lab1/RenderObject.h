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
	/**���屾������**/
	GLuint VBO; //�����洢���嶥�����ݵĻ������(��GPU�У�

	ObjLoader objLoader; //obj�ļ���ȡ�����ݱ���

	GLuint gSampler; //����ȡ�������

	Texture* Texture; //Texture��Ķ����ں� �����������Ŀ�ꡢĬ������Ԫ��

	Transform objectTransformController;

	/**shader���**/
	//shader�ļ���
	string pVSFileName; //vertex shader�ļ���
	string pFSFileName; //fragment shader�ļ���

	//shaderʹ������
	GLuint gWVPLocation; //�任���۾�����ϵ�ı任����
	GLuint gWorldLocation; //�任����������ϵ�ı任����
	GLuint gEyePosition; //�۾������ڵ�����

	Camera* camera;
	//ʹ�õ�shader������
	GLuint ShaderProgram;

	RenderObject(const char* vshader, const char* fshader);
	
	void render();

};