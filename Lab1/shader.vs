#version 330

layout(location = 0) in vec3 Position;	//����Ķ�������	
//layout(location = 1) in vec2 TexCoord;	//�������������
layout(location = 1) in vec3 Normal; //����Ķ��㷨����

//����ı任���۾�����ϵ�ı任����
uniform mat4 gWVP;

//�������
uniform mat4 gWorld; //����ı任����������ϵ�ı任����
uniform vec3 gEye; //������۾�����������ϵ����

//out vec2 TexCoord0;	//�������������
out vec4 Color;

//�������
//out vec3 NormalInWorld; 
//out vec3 PointPosition;
//out vec3 EyePosition;

void main()
{
	//����λ��
	gl_Position = gWVP * vec4(Position, 1.0);

	//�������
	//PointPosition = (gWorld*vec4(Position, 0.0)).xyz;
	//NormalInWorld = (gWorld*vec4(Normal, 0.0)).xyz;
	//EyePosition = (vec4(gEye, 0.0)).xyz;

	//TexCoord0 = TexCoord;
	Color = vec4(clamp(Position, 0.0, 1.0), 1.0);
}