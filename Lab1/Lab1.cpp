// Lab1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "pipeline.h"
#include "camera.h"
//#include "texture.h"
#include "ObjLoader.h"

#include <fstream>
using std::ifstream;
#include <string>
using std::string;


#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT 768


GLuint VBO;
GLuint IBO;
ObjLoader objLoader;
GLuint gWVPLocation;	//传入着色器的全局变量（顶点位置变换矩阵）
//GLuint gSampler;	//传入着色器的全局变量（纹理采样器）

//Texture* pTexture = NULL;
Camera* pGameCamera = NULL;

const char *pVSFileName = "shader.vs";
const char *pFSFileName = "shader.fs";

//用于读取着色器
bool ReadFile(const char *pFileName, string &outFile)
{
	ifstream f(pFileName);

	bool ret = false;

	if (f.is_open())
	{
		string line;
		while (getline(f, line))
		{
			outFile.append(line);
			outFile.append("\n");
		}

		f.close();

		ret = true;
	}
	else
	{
		ret = false;
	}

	return ret;
}


static void RenderSceneCB()
{
	pGameCamera->OnRender();

	glClear(GL_COLOR_BUFFER_BIT);

	static float Scale = 0.0f;

	//Scale += 0.01f;

	//设置物体状态
	Pipeline p;
	p.Rotate(0.0f, Scale, 0.0f);
	p.WorldPos(0.0f, 0.0f, 0.0f);

	p.SetCamera(pGameCamera->GetPos(), pGameCamera->GetTarget(), pGameCamera->GetUp());
	p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.001f, 100.0f);

	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)p.GetTrans());
	
	glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);	//顶点的空间坐标
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);	//顶点的纹理坐标
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//pTexture->Bind(GL_TEXTURE0); 
	glDrawElements(GL_TRIANGLES, objLoader.getIndexBuffer().size(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);

	glutSwapBuffers();
}


static void SpecialKeyboardCB(int Key, int x, int y)
{
	pGameCamera->OnKeyboard(Key);
}


static void KeyboardCB(unsigned char Key, int x, int y)
{
	switch (Key) {
	case 'q':
		exit(0);
	}
}

static void PassiveMouseCB(int x, int y)
{
	pGameCamera->OnMouse(x, y);
}

static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);  //主回调函数
	glutIdleFunc(RenderSceneCB);	//在空闲时的回调

	glutSpecialFunc(SpecialKeyboardCB);	//键盘功能键被按下时的回调
	glutPassiveMotionFunc(PassiveMouseCB); //鼠标的单纯移动（左键、右键等没有被按下的移动）
	glutKeyboardFunc(KeyboardCB);	//键盘常规按键（字母键和数字键）
}

//顶点缓冲器
static void CreateVertexBuffer()
{
	/*Vertex Vertices[4] = { Vertex(Vector3f(-1.0f, -1.0f, 0.5773f)),
						   Vertex(Vector3f(0.0f, -1.0f, -1.15475)),
						   Vertex(Vector3f(1.0f, -1.0f, 0.5773f)),	
						   Vertex(Vector3f(0.0f, 1.0f, 0.0f)) };*/
	vector<Vertex> Vertices = objLoader.getVertexBuffer();
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size()*sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);
}


//索引缓冲器
static void CreateIndexBuffer()
{
	/*unsigned int Indices[] = { 0, 3, 1,
							   1, 3, 2,
							   2, 3, 0,
							   1, 2, 0 };*/
	vector<unsigned int>Indices = objLoader.getIndexBuffer();

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size()*sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	glShaderSource(ShaderObj, 1, p, Lengths);  //读入着色器
	glCompileShader(ShaderObj);
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		exit(1);
	}

	glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShaders()
{
	GLuint ShaderProgram = glCreateProgram();

	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	// 存储着色器文本的字符串缓冲
	string pVs, pFs;

	// 分别读取着色器文件中的文本到字符串缓冲区
	if (!ReadFile(pVSFileName, pVs))
	{
		exit(1);
	};
	if (!ReadFile(pFSFileName, pFs))
	{
		exit(1);
	};

	AddShader(ShaderProgram, pVs.c_str(), GL_VERTEX_SHADER);
	AddShader(ShaderProgram, pFs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };


	//连接着色器并检查程序连接是否有错误
	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	//检查当前管线状态是否良好
	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	//将着色器程序设置到管线声明中
	glUseProgram(ShaderProgram);

	//将顶点坐标的变换矩阵传入着色器程序中的gWVP变量中
	gWVPLocation = glGetUniformLocation(ShaderProgram, "gWVP");
	assert(gWVPLocation != 0xFFFFFFFF);

	//将纹理采样器传入着色器程序中的gSampler变量中
	/*gSampler = glGetUniformLocation(ShaderProgram, "gSampler");
	assert(gSampler != 0xFFFFFFFF);*/
}

int main(int argc, char** argv)
{
	//Magick::InitializeMagick("C:\\source\\vcpkg\\packages\\graphicsmagick_x86-windows\\bin");

	//objLoader.load("C:\\Users\\39471\\Desktop\\cubic.obj");
	objLoader.load("..\\..\\..\\Users\\39471\\Desktop\\SolidWorks\\body.obj");


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	//双缓冲
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Lab1");
	//glutGameModeString("1960x1080");
	//glutEnterGameMode();	//可以拓展为游戏模式

	InitializeGlutCallbacks();

	pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

	// Must be done after glut is initialized!
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//在渲染中把正常视角看不到的面隐藏
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	CreateVertexBuffer();
	CreateIndexBuffer();

	CompileShaders();

	//glUniform1i(gSampler, 0);

	//pTexture = new Texture(GL_TEXTURE_2D, "..\\..\\..\\Users\\39471\\Desktop\\toma2x.png");	//2D目标，文件路径

	/*if (!pTexture->Load()) {
		return 1;
	}*/

	glutMainLoop();

	return 0;
}
