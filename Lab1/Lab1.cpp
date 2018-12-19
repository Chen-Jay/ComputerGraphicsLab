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

GLuint IBO;
vector<unsigned int>Indices;

GLuint gWVPLocation;	//传入着色器的全局变量（变换至眼睛坐标系的变换矩阵）

GLuint gWorldLocation;	//传入的着色器全局变量（变换至世界坐标系的变换矩阵）

GLuint gEyePosition;	//传入的着色器全局变量（摄像机的位置）

GLuint ShaderProgram; //绘制第一个物体的程序

GLuint gSampler;	//传入着色器的全局变量（纹理采样器）

//各个顶点缓冲对象
GLuint VBO1;
GLuint VBO2;
GLuint VBO3;
GLuint VBO4;
GLuint VBO5;
GLuint VBO6;
GLuint VBO7;
GLuint VBO8;

//各个用于读入并保存对应obj文件数据的obj载入器
ObjLoader objLoader1;//Obj文件读入
ObjLoader objLoader2;//Obj文件读入
ObjLoader objLoader3;//Obj文件读入
ObjLoader objLoader4;//Obj文件读入
ObjLoader objLoader5;//Obj文件读入
ObjLoader objLoader6;//Obj文件读入
ObjLoader objLoader7;//Obj文件读入
ObjLoader objLoader8;//Obj文件读入

Light light;//光照管理器

//各个纹理
Texture* pTexture1 = NULL;
Texture* pTexture2 = NULL;
Texture* pTexture3 = NULL;
Texture* pTexture4 = NULL;
Texture* pTexture5 = NULL;
Texture* pTexture6 = NULL;
Texture* pTexture7 = NULL;
Texture* pTexture8 = NULL;

Camera* camera = NULL;

const char *pVSFileName = "vshader.glsl";
const char *pFSFileName = "fshader.glsl";

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
	p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.001f, 1000.0f);

	glUseProgram(ShaderProgram);

	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)(p.GetWorldTrans()->matrix));
	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)(p.GetCameraTrans()->matrix));
	glUniform3f(gEyePosition, camera->GetPos().x, camera->GetPos().y, camera->GetPos().z);

	//一号模型（桌子）
	/*********************************************************************************************************/
	{
		glUniform1i(gSampler, 0);

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
	}
	/*********************************************************************************************************/

	//二号模型（椅子）
	/*********************************************************************************************************/
	{
		glUniform1i(gSampler, 1);

		glBindBuffer(GL_ARRAY_BUFFER, VBO2);	//传入VBO（顶点缓冲对象）

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);	//顶点的空间坐标

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);	//传入IBO（索引缓冲对象）

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);	//顶点的纹理坐标

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20); //顶点的法向量

		glDrawArrays(GL_TRIANGLES, 0, objLoader2.getAllVertexBuffer().size());

		//在不使用顶点和法向量还有纹理的时候，短时间禁用他们会比较好
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
	/*********************************************************************************************************/

	//三号模型（飞机）
	/*********************************************************************************************************/
	{
		glUniform1i(gSampler, 2);

		glBindBuffer(GL_ARRAY_BUFFER, VBO3);	//传入VBO（顶点缓冲对象）

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);	//顶点的空间坐标

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);	//传入IBO（索引缓冲对象）

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);	//顶点的纹理坐标

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20); //顶点的法向量

		glDrawArrays(GL_TRIANGLES, 0, objLoader3.getAllVertexBuffer().size());

		//在不使用顶点和法向量还有纹理的时候，短时间禁用他们会比较好
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
	/*********************************************************************************************************/

	//四号模型（屏幕）
	/*********************************************************************************************************/
	{
		glUniform1i(gSampler, 3);

		glBindBuffer(GL_ARRAY_BUFFER, VBO4);	//传入VBO（顶点缓冲对象）

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);	//顶点的空间坐标

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);	//传入IBO（索引缓冲对象）

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);	//顶点的纹理坐标

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20); //顶点的法向量

		glDrawArrays(GL_TRIANGLES, 0, objLoader4.getAllVertexBuffer().size());

		//在不使用顶点和法向量还有纹理的时候，短时间禁用他们会比较好
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
	/*********************************************************************************************************/

	//五号模型（笔记本）
	/*********************************************************************************************************/
	{
		glUniform1i(gSampler, 4);

		glBindBuffer(GL_ARRAY_BUFFER, VBO5);	//传入VBO（顶点缓冲对象）

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);	//顶点的空间坐标

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);	//传入IBO（索引缓冲对象）

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);	//顶点的纹理坐标

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20); //顶点的法向量

		glDrawArrays(GL_TRIANGLES, 0, objLoader5.getAllVertexBuffer().size());

		//在不使用顶点和法向量还有纹理的时候，短时间禁用他们会比较好
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
	/*********************************************************************************************************/

	//六号模型（身体）
	/*********************************************************************************************************/
	{
		glUniform1i(gSampler, 5);

		glBindBuffer(GL_ARRAY_BUFFER, VBO6);	//传入VBO（顶点缓冲对象）

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);	//顶点的空间坐标

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);	//传入IBO（索引缓冲对象）

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);	//顶点的纹理坐标

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20); //顶点的法向量

		glDrawArrays(GL_TRIANGLES, 0, objLoader6.getAllVertexBuffer().size());

		//在不使用顶点和法向量还有纹理的时候，短时间禁用他们会比较好
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
	/*********************************************************************************************************/

	//七号模型（头）
	/*********************************************************************************************************/
	{
		glUniform1i(gSampler, 6);

		glBindBuffer(GL_ARRAY_BUFFER, VBO7);	//传入VBO（顶点缓冲对象）

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);	//顶点的空间坐标

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);	//传入IBO（索引缓冲对象）

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);	//顶点的纹理坐标

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20); //顶点的法向量

		glDrawArrays(GL_TRIANGLES, 0, objLoader7.getAllVertexBuffer().size());

		//在不使用顶点和法向量还有纹理的时候，短时间禁用他们会比较好
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
	/*********************************************************************************************************/

	//八号模型（天空球）
	/*********************************************************************************************************/
	{
		glUniform1i(gSampler, 7);

		glBindBuffer(GL_ARRAY_BUFFER, VBO8);	//传入VBO（顶点缓冲对象）

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);	//顶点的空间坐标

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);	//传入IBO（索引缓冲对象）

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);	//顶点的纹理坐标

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20); //顶点的法向量

		glDrawArrays(GL_TRIANGLES, 0, objLoader8.getAllVertexBuffer().size());

		//在不使用顶点和法向量还有纹理的时候，短时间禁用他们会比较好
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
	/*********************************************************************************************************/

	//清楚深度检测的缓存
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
static void CreateVertexBuffer(GLuint& VBO,ObjLoader& objLoader)
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, objLoader.getAllVertexBuffer().size() * sizeof(Vertex), &objLoader.getAllVertexBuffer()[0], GL_STATIC_DRAW);
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
static void CreateProgram()
{
	ShaderProgram = glCreateProgram();

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

	//将变换至世界坐标系的变换矩阵传入着色器程序中的gWorld变量中
	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
	assert(gWorldLocation != 0xFFFFFFFF);

	//将变换至世界坐标系的变换矩阵传入着色器程序中的gWorld变量中
	gEyePosition = glGetUniformLocation(ShaderProgram, "gEye");
	assert(gEyePosition != 0xFFFFFFFF);

	//将纹理采样器传入着色器程序中的gSampler变量中
	gSampler = glGetUniformLocation(ShaderProgram, "gSampler");
	assert(gSampler != 0xFFFFFFFF);
}

/*主函数*/
int main(int argc, char** argv)
{
	//Magick::InitializeMagick("C:\\source\\vcpkg\\packages\\graphicsmagick_x86-windows\\bin");


	//obj模型读入
	objLoader1.load("C:\\Users\\39471\\Desktop\\第四次实验\\导入素材\\desk.obj");
	objLoader2.load("C:\\Users\\39471\\Desktop\\第四次实验\\导入素材\\chair.obj");
	objLoader3.load("C:\\Users\\39471\\Desktop\\第四次实验\\导入素材\\plane.obj");
	objLoader4.load("C:\\Users\\39471\\Desktop\\第四次实验\\导入素材\\screen.obj");
	objLoader5.load("C:\\Users\\39471\\Desktop\\第四次实验\\导入素材\\desktop.obj");
	objLoader6.load("C:\\Users\\39471\\Desktop\\第四次实验\\导入素材\\body.obj");
	objLoader7.load("C:\\Users\\39471\\Desktop\\第四次实验\\导入素材\\head.obj");
	objLoader8.load("C:\\Users\\39471\\Desktop\\第四次实验\\导入素材\\skyball.obj");

	//窗口设置
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	//双缓冲
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Lab4");

	//创建相机对象
	camera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	
	//创建对应的缓冲器
	CreateVertexBuffer(VBO1, objLoader1);
	CreateVertexBuffer(VBO2, objLoader2);
	CreateVertexBuffer(VBO3, objLoader3);
	CreateVertexBuffer(VBO4, objLoader4);
	CreateVertexBuffer(VBO5, objLoader5);
	CreateVertexBuffer(VBO6, objLoader6);
	CreateVertexBuffer(VBO7, objLoader7);
	CreateVertexBuffer(VBO8, objLoader8);
	//CreateIndexBuffer();

	//light.CalcNormals_Obj(&Vertices1);

	//加载、编译着色器
	CreateProgram();	

	/*纹理生成与绑定*/
	//根据纹理图片和纹理目标创建一个Texture类
	pTexture1 = new Texture(GL_TEXTURE_2D, "C:\\users\\39471\\Desktop\\第四次实验\\导入素材\\desk_texture.png", GL_TEXTURE0);
	pTexture2 = new Texture(GL_TEXTURE_2D, "C:\\users\\39471\\Desktop\\第四次实验\\导入素材\\chair_texture.jpg", GL_TEXTURE1);
	pTexture3 = new Texture(GL_TEXTURE_2D, "C:\\users\\39471\\Desktop\\第四次实验\\导入素材\\plane_texture2.bmp", GL_TEXTURE2);
	pTexture4 = new Texture(GL_TEXTURE_2D, "C:\\users\\39471\\Desktop\\第四次实验\\导入素材\\screen_texture.png", GL_TEXTURE3);
	pTexture5 = new Texture(GL_TEXTURE_2D, "C:\\users\\39471\\Desktop\\第四次实验\\导入素材\\desktop_texture2.jpg", GL_TEXTURE4);
	pTexture6 = new Texture(GL_TEXTURE_2D, "C:\\users\\39471\\Desktop\\第四次实验\\导入素材\\cloth2.png", GL_TEXTURE5);
	pTexture7 = new Texture(GL_TEXTURE_2D, "C:\\users\\39471\\Desktop\\第四次实验\\导入素材\\head2.png", GL_TEXTURE6);
	pTexture8 = new Texture(GL_TEXTURE_2D, "C:\\users\\39471\\Desktop\\第四次实验\\导入素材\\skyball_texture.jpg", GL_TEXTURE7);

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
	
	if (!pTexture3->Load())
	{
		return 1;
	}
	else
	{
		pTexture3->Bind(GL_TEXTURE2);
	}

	if (!pTexture4->Load())
	{
		return 1;
	}
	else
	{
		pTexture4->Bind(GL_TEXTURE3);
	}

	if (!pTexture5->Load())
	{
		return 1;
	}
	else
	{
		pTexture5->Bind(GL_TEXTURE4);
	}

	if (!pTexture6->Load())
	{
		return 1;
	}
	else
	{
		pTexture6->Bind(GL_TEXTURE5);
	}

	if (!pTexture7->Load())
	{
		return 1;
	}
	else
	{
		pTexture7->Bind(GL_TEXTURE6);
	}

	if (!pTexture8->Load())
	{
		return 1;
	}
	else
	{
		pTexture8->Bind(GL_TEXTURE7);
	}

	glEnable(GL_DEPTH_TEST);

	//在渲染中把正常视角看不到的面隐藏
	/*glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);*/
	
	//设置背景颜色
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

	//注册回调
	InitializeGlutCallbacks();

	glutMainLoop();

	return 0;
}
