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

#include <fstream>
using std::ifstream;
#include <string>
using std::string;


#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT 768

GLuint VBO1;
GLuint VBO2;

vector<Vertex> Vertices1;
vector<Vertex> Vertices2;

GLuint IBO;
vector<unsigned int>Indices;

GLuint gWVPLocation1;	//传入着色器的全局变量（变换至眼睛坐标系的变换矩阵）
GLuint gWVPLocation2;   //传入着色器的全局变量（变换至眼睛坐标系的变换矩阵）

GLuint gWorldLocation1;	//传入的着色器全局变量（变换至世界坐标系的变换矩阵）
GLuint gWorldLocation2;	//传入的着色器全局变量（变换至世界坐标系的变换矩阵）

GLuint gEyePosition1;	//传入的着色器全局变量（摄像机的位置）
GLuint gEyePosition2;	//传入的着色器全局变量（摄像机的位置）


GLuint ShaderProgram1; //绘制第一个物体的程序
GLuint ShaderProgram2; //绘制第二个物体的程序

GLuint gSampler1;	//传入着色器的全局变量（纹理采样器）
GLuint gSampler2;	//传入着色器的全局变量（纹理采样器）

ObjLoader objLoader1;//Obj文件读入
ObjLoader objLoader2;//Obj文件读入


Light light;//光照管理器


Texture* pTexture1 = NULL;
Texture* pTexture2 = NULL;

Camera* camera = NULL;

const char *pVSFileName1 = "vshader1.glsl";
const char *pFSFileName1 = "fshader1.glsl";
const char *pVSFileName2 = "vshader2.glsl";
const char *pFSFileName2 = "fshader2.glsl";

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

	static float Scale = 0.0f;

	//Scale += 0.01f;

	//设置物体状态
	Transform p;
	p.Rotate(0.0f, Scale, 0.0f);
	p.WorldPos(0.0f, 0.0f, 0.0f);

	p.SetCamera(camera->GetPos(), camera->GetTarget(), camera->GetUp());
	p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.001f, 100.0f);

	//一号模型
	/*********************************************************************************************************/
	glUseProgram(ShaderProgram1);

	glUniformMatrix4fv(gWorldLocation1, 1, GL_TRUE, (const GLfloat*)(p.GetWorldTrans()->matrix));
	glUniformMatrix4fv(gWVPLocation1, 1, GL_TRUE, (const GLfloat*)(p.GetCameraTrans()->matrix));
	glUniform3f(gEyePosition1, camera->GetPos().x, camera->GetPos().y, camera->GetPos().z);

	glUniform1i(gSampler1, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO1);	//传入VBO（顶点缓冲对象）

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);	//顶点的空间坐标

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);	//传入IBO（索引缓冲对象）

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);	//顶点的纹理坐标

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20); //顶点的法向量

	glDrawArrays(GL_TRIANGLES, 0, objLoader1.getAllVertexBuffer().size());

	//在不使用顶点和法向量还有纹理的时候，短时间禁用他们会比较好
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	/*********************************************************************************************************/

	//二号模型
	/*********************************************************************************************************/
	glUseProgram(ShaderProgram2);

	glUniformMatrix4fv(gWorldLocation2, 1, GL_TRUE, (const GLfloat*)(p.GetWorldTrans()->matrix));
	glUniformMatrix4fv(gWVPLocation2, 1, GL_TRUE, (const GLfloat*)(p.GetCameraTrans()->matrix));
	glUniform3f(gEyePosition2, camera->GetPos().x, camera->GetPos().y, camera->GetPos().z);

	glUniform1i(gSampler2, 1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);	//传入VBO（顶点缓冲对象）

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);	//顶点的空间坐标

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);	//传入IBO（索引缓冲对象）

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);	//顶点的纹理坐标

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20); //顶点的法向量

	glDrawArrays(GL_TRIANGLES, 0, objLoader2.getAllVertexBuffer().size());

	//glDrawElements(GL_TRIANGLES, objLoader.getIndexBuffer().size(), GL_UNSIGNED_INT, 0);

	//在不使用顶点和法向量还有纹理的时候，短时间禁用他们会比较好
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);	
	/*********************************************************************************************************/

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
static void CreateVertexBuffer1()
{
	Vertices1 = objLoader1.getAllVertexBuffer();
	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, Vertices1.size() * sizeof(Vertex), &Vertices1[0], GL_STATIC_DRAW);
}

static void CreateVertexBuffer2()
{
	Vertices2 = objLoader2.getAllVertexBuffer();
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, Vertices2.size() * sizeof(Vertex), &Vertices2[0], GL_STATIC_DRAW);
}

//2.索引缓冲器
static void CreateIndexBuffer()
{
	Indices = objLoader1.getIndexBuffer();

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
static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
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
static void CreateProgram1()
{
	ShaderProgram1 = glCreateProgram();

	if (ShaderProgram1 == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	// 存储着色器文本的字符串缓冲
	string pVs, pFs;

	// 分别读取着色器文件中的文本到字符串缓冲区
	if (!ReadFile(pVSFileName1, pVs))
	{
		exit(1);
	};
	if (!ReadFile(pFSFileName1, pFs))
	{
		exit(1);
	};

	AddShader(ShaderProgram1, pVs.c_str(), GL_VERTEX_SHADER);
	AddShader(ShaderProgram1, pFs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };


	//连接着色器并检查程序连接是否有错误
	glLinkProgram(ShaderProgram1);
	glGetProgramiv(ShaderProgram1, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram1, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	//检查当前管线状态是否良好
	glValidateProgram(ShaderProgram1);
	glGetProgramiv(ShaderProgram1, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram1, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	//将着色器程序设置到管线声明中
	glUseProgram(ShaderProgram1);

	//将顶点坐标的变换矩阵传入着色器程序中的gWVP变量中
	gWVPLocation1 = glGetUniformLocation(ShaderProgram1, "gWVP");
	assert(gWVPLocation1 != 0xFFFFFFFF);

	//将变换至世界坐标系的变换矩阵传入着色器程序中的gWorld变量中
	gWorldLocation1 = glGetUniformLocation(ShaderProgram1, "gWorld");
	assert(gWorldLocation1 != 0xFFFFFFFF);

	//将变换至世界坐标系的变换矩阵传入着色器程序中的gWorld变量中
	gEyePosition1 = glGetUniformLocation(ShaderProgram1, "gEye");
	assert(gEyePosition1 != 0xFFFFFFFF);

	//将纹理采样器传入着色器程序中的gSampler变量中
	gSampler1 = glGetUniformLocation(ShaderProgram1, "gSampler");
	assert(gSampler1 != 0xFFFFFFFF);
}

static void CreateProgram2()
{
	ShaderProgram2 = glCreateProgram();

	if (ShaderProgram2 == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	// 存储着色器文本的字符串缓冲
	string pVs, pFs;

	// 分别读取着色器文件中的文本到字符串缓冲区
	if (!ReadFile(pVSFileName2, pVs))
	{
		exit(1);
	};
	if (!ReadFile(pFSFileName2, pFs))
	{
		exit(1);
	};

	AddShader(ShaderProgram2, pVs.c_str(), GL_VERTEX_SHADER);
	AddShader(ShaderProgram2, pFs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };


	//连接着色器并检查程序连接是否有错误
	glLinkProgram(ShaderProgram2);
	glGetProgramiv(ShaderProgram2, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram2, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	//检查当前管线状态是否良好
	glValidateProgram(ShaderProgram2);
	glGetProgramiv(ShaderProgram2, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram2, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	//将着色器程序设置到管线声明中
	glUseProgram(ShaderProgram2);

	//将顶点坐标的变换矩阵传入着色器程序中的gWVP变量中
	gWVPLocation2 = glGetUniformLocation(ShaderProgram2, "gWVP");
	assert(gWVPLocation2 != 0xFFFFFFFF);

	//将变换至世界坐标系的变换矩阵传入着色器程序中的gWorld变量中
	gWorldLocation2 = glGetUniformLocation(ShaderProgram2, "gWorld");
	assert(gWorldLocation2 != 0xFFFFFFFF);

	//将变换至世界坐标系的变换矩阵传入着色器程序中的gWorld变量中
	gEyePosition2 = glGetUniformLocation(ShaderProgram2, "gEye");
	assert(gEyePosition2 != 0xFFFFFFFF);

	//将纹理采样器传入着色器程序中的gSampler变量中
	gSampler2 = glGetUniformLocation(ShaderProgram2, "gSampler");
	assert(gSampler2 != 0xFFFFFFFF);
}

/*主函数*/
int main(int argc, char** argv)
{
	//Magick::InitializeMagick("C:\\source\\vcpkg\\packages\\graphicsmagick_x86-windows\\bin");


	//obj模型读入
	objLoader1.load("C:\\Users\\39471\\Desktop\\house_C1_light&texture.obj");
	objLoader2.load("C:\\Users\\39471\\Desktop\\brickHouse.obj");

	//窗口设置
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	//双缓冲
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Lab1");

	//创建相机对象
	camera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	
	//创建对应的缓冲器
	CreateVertexBuffer1();
	CreateVertexBuffer2();
	//CreateIndexBuffer();

	//light.CalcNormals_Obj(&Vertices1);

	//加载、编译着色器
	CreateProgram1();	
	CreateProgram2();

	/*纹理生成与绑定*/
	//根据纹理图片和纹理目标创建一个Texture类
	pTexture1 = new Texture(GL_TEXTURE_2D, "C:\\users\\39471\\Desktop\\wood.jpg", GL_TEXTURE0);
	pTexture2 = new Texture(GL_TEXTURE_2D, "C:\\users\\39471\\Desktop\\brick.jpg", GL_TEXTURE1);


	//纹理采样器有了，纹理图片和目标也知道，下面就是生成纹理对象，让对象和目标绑定在一起，把图片数据解析出来，读入目标中
	if (!pTexture1->Load()) {
		return 1;
	}
	else
	{
		pTexture1->Bind(GL_TEXTURE0);
	}	

	if (!pTexture2->Load())
	{
		return 1;
	}
	else
	{
		pTexture2->Bind(GL_TEXTURE1);
	}
	
	//glEnable(GL_DEPTH_TEST);

	//在渲染中把正常视角看不到的面隐藏
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	
	//设置背景颜色
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

	//注册回调
	InitializeGlutCallbacks();

	glutMainLoop();

	return 0;
}
