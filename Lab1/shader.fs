#version 330

in vec4 Color;
//in vec2 TexCoord0;	//�������������

out vec4 FragColor;

//uniform sampler2D gSampler;	//����Ĳ�����ȫ�ֱ���


void main()
{
	//FragColor = texture2D(gSampler, TexCoord0.st);
	FragColor = Color;
}