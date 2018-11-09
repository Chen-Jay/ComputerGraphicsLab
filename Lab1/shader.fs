#version 330

in vec4 Color;
//in vec2 TexCoord0;	//传入的纹理坐标

out vec4 FragColor;

//uniform sampler2D gSampler;	//传入的采样器全局变量


void main()
{
	//FragColor = texture2D(gSampler, TexCoord0.st);
	FragColor = Color;
}