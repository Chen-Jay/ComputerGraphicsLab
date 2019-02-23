#version 330

layout(location = 0) in vec3 Position;	//传入的顶点坐标	
layout(location = 1) in vec2 TexturePosition;	//传入的纹理坐标
layout(location = 2) in vec3 Normal; //传入的顶点法向量

//传入的变换到眼睛坐标系的变换矩阵
uniform mat4 gWVP;

//光照相关
uniform mat4 gWorld; //传入的变换到世界坐标系的变换矩阵
uniform vec3 gEye; //传入的眼睛的世界坐标系坐标

out vec2 TextureAfterTrans;	//输出的纹理坐标
//out vec4 Color;

//光线相关
out vec3 NormalInWorld; 
out vec3 PointPosition;
out vec3 EyePosition;

void main()
{
	//顶点位置输出
	gl_Position = gWVP * vec4(Position, 1.0);

	//光照相关
	PointPosition = (vec4(Position, 0.0)).xyz; //世界坐标系顶点坐标传递
	NormalInWorld = (gWorld*vec4(Normal, 0.0)).xyz;//世界坐标系法向量传递
	EyePosition = (vec4(gEye, 0.0)).xyz;//世界坐标系眼睛坐标传递

	TextureAfterTrans = TexturePosition;//纹理坐标系坐标传递
	//Color = vec4(clamp(Position, 0.0, 1.0), 1.0);
}