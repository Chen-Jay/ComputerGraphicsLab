#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "transform.h"
#include "camera.h"
#include "texture.h"
#include "Light.h"
#include "ObjLoader.h"

#include "RenderObject.h"

#include <fstream>
using std::ifstream;
#include <string>
using std::string;


#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT 768

GLuint IBO;
vector<unsigned int>Indices;

Light light;//光照管理器

Camera* camera = NULL;

RenderObject woodhouse("vshader1.glsl", "fshader1.glsl");
RenderObject brickhouse("vshader2.glsl", "fshader2.glsl");

static float Scale = 0.0f;

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

/*主渲染*/
static void RenderSceneCB()
{
	camera->OnRender();

	glClear(GL_COLOR_BUFFER_BIT);

	Scale += 0.1f;

	//设置物体进行变换的状态、传入相机位置
	woodhouse.objectTransformController.Rotate(0.0f, 0.0f, 0.0f);
	woodhouse.objectTransformController.WorldPos(0.0f, 0.0f, 0.0f);
	woodhouse.objectTransformController.SetCamera(camera->GetPos(), camera->GetTarget(), camera->GetUp());
	woodhouse.objectTransformController.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.001f, 100.0f);

	brickhouse.objectTransformController.Rotate(0.0f, 0.0, 0.0f);
	brickhouse.objectTransformController.WorldPos(0.0f, 0.0f, 0.0f);
	brickhouse.objectTransformController.SetCamera(camera->GetPos(), camera->GetTarget(), camera->GetUp());
	brickhouse.objectTransformController.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.001f, 100.0f);

	/**开始渲染***/
	woodhouse.render();
	brickhouse.render();
	/**结束渲染***/

	//深度检测（每一帧都要清除掉深度缓存）
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	glutSwapBuffers();//双缓冲
}

/*各种回调函数*/
//1.键盘功能区域监听
static void SpecialKeyboardCB(int Key, int x, int y)
{
	camera->OnKeyboard(Key);
}
//2.键盘普通区域监听
static void KeyboardCB(unsigned char Key, int x, int y)
{
	switch (Key) {
	case 'q':
		exit(0);
	}
}
//3.鼠标（未点击情况下）移动监听
static void PassiveMouseCB(int x, int y)
{
	camera->OnMouse(x, y);
}
//4.注册各个回调函数
static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);  //主回调函数
	glutIdleFunc(RenderSceneCB);	//在空闲时的回调

	glutSpecialFunc(SpecialKeyboardCB);	//键盘功能键被按下时的回调
	glutPassiveMotionFunc(PassiveMouseCB); //鼠标的单纯移动（左键、右键等没有被按下的移动）
	glutKeyboardFunc(KeyboardCB);	//键盘常规按键（字母键和数字键）
}


/*缓冲器和纹理对象*/
//1.顶点缓冲器
static void CreateVertexBuffer(ObjLoader& objLoader,GLuint& VBO)
{
	vector<Vertex> Vertices = objLoader.getAllVertexBuffer();
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);
}

//2.索引缓冲器
static void CreateIndexBuffer(ObjLoader& objLoader, GLuint& IBO)
{
	Indices = objLoader.getIndexBuffer();

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);
}

//3.纹理对象
static void CreatTextureObjectA() {

}

/*Shader相关*/

/**
 * 1.用于读入shader代码
 * shaderprogram:对应的shader程序
 * pShaderText：shader的代码（字符串形式
 * shaderType：shader类型
 **/
static void AddShader(GLuint& ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}

	const GLchar* ShaderContent[1];
	ShaderContent[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	glShaderSource(ShaderObj, 1, ShaderContent, Lengths);  //读入着色器
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

//2.传入各种变量，编译shader代码
static void CreateProgram(RenderObject& renderObject)
{
	renderObject.ShaderProgram = glCreateProgram();

	if (renderObject.ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	// 存储着色器文本的字符串缓冲
	string pVs, pFs;

	// 分别读取着色器文件中的文本到字符串缓冲区
	if (!ReadFile(renderObject.pVSFileName.c_str(), pVs))
	{
		exit(1);
	};
	if (!ReadFile(renderObject.pFSFileName.c_str(), pFs))
	{
		exit(1);
	};

	AddShader(renderObject.ShaderProgram, pVs.c_str(), GL_VERTEX_SHADER);
	AddShader(renderObject.ShaderProgram, pFs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };


	//连接着色器并检查程序连接是否有错误
	glLinkProgram(renderObject.ShaderProgram);
	glGetProgramiv(renderObject.ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(renderObject.ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	//检查当前管线状态是否良好
	glValidateProgram(renderObject.ShaderProgram);
	glGetProgramiv(renderObject.ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(renderObject.ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	//将着色器程序设置到管线声明中
	glUseProgram(renderObject.ShaderProgram);

	//将顶点坐标的变换矩阵传入着色器程序中的gWVP变量中
	renderObject.gWVPLocation = glGetUniformLocation(renderObject.ShaderProgram, "gWVP");
	assert(renderObject.gWVPLocation != 0xFFFFFFFF);

	//将变换至世界坐标系的变换矩阵传入着色器程序中的gWorld变量中
	renderObject.gWorldLocation = glGetUniformLocation(renderObject.ShaderProgram, "gWorld");
	assert(renderObject.gWorldLocation != 0xFFFFFFFF);

	//将变换至眼睛坐标系的变换矩阵传入着色器程序中的gWorld变量中
	renderObject.gEyePosition = glGetUniformLocation(renderObject.ShaderProgram, "gEye");
	assert(renderObject.gEyePosition != 0xFFFFFFFF);

	//将纹理采样器传入着色器程序中的gSampler变量中
	renderObject.gSampler = glGetUniformLocation(renderObject.ShaderProgram, "gSampler");
	assert(renderObject.gSampler != 0xFFFFFFFF);
}

/*主函数*/
int main(int argc, char** argv)
{
	//Magick::InitializeMagick("C:\\source\\vcpkg\\packages\\graphicsmagick_x86-windows\\bin");

	//obj模型读入
	woodhouse.objLoader.load("C:\\Users\\39471\\Desktop\\house_C1_light&texture.obj");
	brickhouse.objLoader.load("C:\\Users\\39471\\Desktop\\brickHouse.obj");

	//窗口设置
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	//双缓冲
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Lab3");

	//创建相机对象
	camera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);
	//将相机对象的指针传给每个需要渲染的对象（需要借助相机的信息计算变换至眼睛坐标系的变换矩阵用于渲染）
	woodhouse.camera = camera;
	brickhouse.camera = camera;

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	
	//创建对应的缓冲器
	CreateVertexBuffer(woodhouse.objLoader,woodhouse.VBO);
	CreateVertexBuffer(brickhouse.objLoader,brickhouse.VBO);

	//CreateIndexBuffer();  //索引缓冲器，因为暂时用不上索引绘制，所以先注释掉

	//加载、编译着色器
	CreateProgram(woodhouse);	
	CreateProgram(brickhouse);

	//根据纹理图片和纹理目标创建一个Texture类
	woodhouse.Texture = new Texture(GL_TEXTURE_2D, "C:\\users\\39471\\Desktop\\wood.jpg", GL_TEXTURE0,0);
	brickhouse.Texture = new Texture(GL_TEXTURE_2D, "C:\\users\\39471\\Desktop\\brick.jpg", GL_TEXTURE1,1);

	//产生纹理并将其绑入对应的纹理单元
	if (!woodhouse.Texture->Load()) {
		return 1;
	}
	else
	{
		woodhouse.Texture->Bind(GL_TEXTURE0);
	}	

	if (!brickhouse.Texture->Load())
	{
		return 1;
	}
	else
	{
		brickhouse.Texture->Bind(GL_TEXTURE1);
	}
	
	
	//在渲染中把正常视角看不到的面隐藏
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	
	//设置背景颜色(有点类似于灰色)
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

	//注册回调
	InitializeGlutCallbacks();

	glutMainLoop();

	return 0;
}
