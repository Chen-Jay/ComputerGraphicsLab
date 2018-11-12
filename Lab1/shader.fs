#version 330

in vec4 Color;

//in vec2 TexCoord0;	//传入的纹理坐标

//光照相关
//in vec3 PointPosition;
//in vec3 NormalInWorld; //传入的世界坐标系下的发现坐标
//in vec3 EyePosition;

out vec4 FragColor;

//uniform sampler2D gSampler;	//传入的采样器全局变量

//平行光源
struct DirectionalLight
{
	vec3 Color;//红光
	float AmbientIntensity; //环境光系数
	float DiffuseIntensity;//漫射光光强
	vec3 Direction;//光照方向
};

//点光源
struct PointLight
{
	vec3 Color;
	float DiffuseIntensity; //漫反射系数
	float AmbientIntensity; //环境光系数
	float SpecularIntensity; //反射光系数
	float Attenuation_constant; //常数衰减系数
	float Attenuation_liner; //线性衰减系数
	float Attenuation_exp;  //指数衰减系数
	vec3 position; //点光源位置
};

vec4 CaculateDiffuseColor(PointLight light, vec3 PointPosition, vec3 NormalInWorld)
{
	vec3 LightDirection = light.position-PointPosition;
	float LightDistance = length(LightDirection);
	float LightDirectionFactor =dot(normalize(NormalInWorld),LightDirection);
	LightDirectionFactor = LightDirectionFactor>0.0?LightDirectionFactor:0.0;
	LightDirectionFactor = LightDirectionFactor<1.0?LightDirectionFactor:1.0;

	return vec4(light.Color * light.DiffuseIntensity * LightDirectionFactor, 1.0f)/
						(light.Attenuation_constant + light.Attenuation_liner * LightDistance + light.Attenuation_exp * LightDistance * LightDistance);

};

vec4 CaculateSpecularColor(PointLight light, vec3 PointPosition, vec3 NormalInWorld,vec3 EyePosition ,float SpecularPower)
{
	vec3 VertexToEye= normalize(EyePosition - PointPosition);
	vec3 LightDirection = -(light.position-PointPosition);	
	vec3 LightReflect = normalize(reflect(LightDirection, NormalInWorld));
	
	
	
	float SpecularFactor = dot(VertexToEye, LightReflect);

	if (SpecularFactor > 0) 
	{
        SpecularFactor = pow(SpecularFactor, SpecularPower);
		return vec4(light.Color * light.SpecularIntensity * SpecularFactor, 1.0f);
    }
	else
	{
		return vec4(0.0,0.0,0.0,0.0);
	}
};

void main()
{
	//FragColor = texture2D(gSampler, TexCoord0.st);

	//PointLight RedPointLight;
	//RedPointLight.Color=vec3(1.0,0.0,0.0);
	//RedPointLight.AmbientIntensity=0.3;
	//RedPointLight.DiffuseIntensity=1.0;
	//RedPointLight.SpecularIntensity=0.5;
	//RedPointLight.Attenuation_constant= 0.5;
	//RedPointLight.Attenuation_liner= 0.2;
	//RedPointLight.Attenuation_exp= 0.2;
	//RedPointLight.position=vec3(0.0,1.5,0.0);

	//PointLight GreenPointLight;
	//GreenPointLight.Color=vec3(0.0,1.0,0.0);
	//GreenPointLight.AmbientIntensity=0.3;
	//GreenPointLight.DiffuseIntensity= 1.0;
	//GreenPointLight.SpecularIntensity= 0.5;
	//GreenPointLight.Attenuation_constant= 0.5;
	//GreenPointLight.Attenuation_liner= 0.2;
	//GreenPointLight.Attenuation_exp= 0.2;
	//GreenPointLight.position=vec3(-1.5,0.0,0.0);

	//PointLight BluePointLight;
	//BluePointLight.Color=vec3(0.0,0.0,1.0);
	//BluePointLight.AmbientIntensity=0.3;
	//BluePointLight.DiffuseIntensity=1.0;
	//BluePointLight.SpecularIntensity=0.5;
	//BluePointLight.Attenuation_constant= 0.5;
	//BluePointLight.Attenuation_liner= 0.2;
	//BluePointLight.Attenuation_exp= 0.2;
	//BluePointLight.position=vec3(0.0,0.0,-1.5);

	//vec4 MatarialColor = vec4(1.0,1.0,1.0,1.0);//定义材质颜色

	////计算环境光光强
	//vec4 AmbientColor = vec4(RedPointLight.Color * RedPointLight.AmbientIntensity, 1.0f)+
	//					vec4(BluePointLight.Color * BluePointLight.AmbientIntensity, 1.0f)+
	//					vec4(GreenPointLight.Color * GreenPointLight.AmbientIntensity, 1.0f); 

	////计算漫射光强
	//vec4 DiffuseColor1 = CaculateDiffuseColor(RedPointLight, PointPosition, NormalInWorld);
	//vec4 DiffuseColor2 = CaculateDiffuseColor(GreenPointLight, PointPosition, NormalInWorld);
	//vec4 DiffuseColor3 = CaculateDiffuseColor(BluePointLight, PointPosition, NormalInWorld);
	//
	////计算镜面反射光
	//vec4 SpecularColor1 = CaculateSpecularColor(RedPointLight, PointPosition, NormalInWorld,EyePosition,0.5);
	//vec4 SpecularColor2 = CaculateSpecularColor(GreenPointLight, PointPosition, NormalInWorld,EyePosition,0.5);
	//vec4 SpecularColor3 = CaculateSpecularColor(BluePointLight, PointPosition, NormalInWorld,EyePosition,0.5);

	//FragColor = MatarialColor * (
	//		AmbientColor + 
	//		DiffuseColor1 + DiffuseColor2 + DiffuseColor3+
	//		SpecularColor1+ SpecularColor2+ SpecularColor3);

	FragColor = Color;
}