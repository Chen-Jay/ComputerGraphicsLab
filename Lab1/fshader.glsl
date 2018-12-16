#version 330

//in vec4 Color;

in vec2 TextureAfterTrans;	//传入的经过插值的纹理坐标

//光照相关
in vec3 PointPosition; //传入的世界坐标系下物体的坐标
in vec3 NormalInWorld; //传入的世界坐标系下物体的法线坐标
in vec3 EyePosition; //传入的世界坐标系下眼睛的坐标

out vec4 FragColor;

uniform sampler2D gSampler;	//传入的采样器全局变量

//平行光源
struct DirectionalLight
{
	vec3 Color;//红光
	float AmbientIntensity; //环境光系数
	float DiffuseIntensity;//漫射光光强
	float SpecularIntensity;//镜面反射光光强
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

//漫射光最后的效果计算(光源是点光源)
vec4 CaculateDiffuseColor_pointLight(PointLight light, vec3 PointPosition, vec3 NormalInWorld)
{
	//入射光线向量（从光源指向当前物体上正在处理的点）(暂时不进行单位化，因为需要用来计算距离）
	vec3 LightDirection = PointPosition - light.position;
	//得到光源和物体上的点的距离
	float LightDistance = length(LightDirection);
	//点乘正在处理的点的法线和光照方向得到漫射光的具体光强（此时需要进行单位化，因为是用来计算夹角）
	float LightDirectionFactor =dot(normalize(NormalInWorld), normalize(-LightDirection));
	//判定得到的具体光强系数，如果小于0，则说明光照是从物体背面传来的，因此将光照系数赋为0，如果大于一(理论上不会，则将其限制为1）
	LightDirectionFactor = LightDirectionFactor>0.0?LightDirectionFactor:0.0;
	LightDirectionFactor = LightDirectionFactor<1.0?LightDirectionFactor:1.0;
	//最后的光的效果是 （漫射光的颜色*漫射光的影响系数*漫射光的具体光强系数）/ （点光源的常数衰减系数+光照距离*点光源的线性衰减系数+光照距离的平方*点光源的指数衰减系数）
	return vec4(light.Color * light.DiffuseIntensity * LightDirectionFactor, 1.0f)/
						(light.Attenuation_constant + light.Attenuation_liner * LightDistance + light.Attenuation_exp * LightDistance * LightDistance);
	//return vec4(light.Color * light.DiffuseIntensity * LightDirectionFactor, 1.0f);
};

//漫射光最后的效果计算(光源是平行光源)
vec4 CaculateDiffuseColor_directionLight(DirectionalLight light, vec3 PointPosition, vec3 NormalInWorld)
{
	//点乘正在处理的点的法线和光照方向得到漫射光的具体光强
	float LightDirectionFactor = dot(normalize(NormalInWorld), normalize(light.Direction));
	//判定得到的具体光强系数，如果小于0，则说明光照是从物体背面传来的，因此将光照系数赋为0，如果大于一(理论上不会，则将其限制为1）
	LightDirectionFactor = LightDirectionFactor > 0.0 ? LightDirectionFactor : 0.0;
	LightDirectionFactor = LightDirectionFactor < 1.0 ? LightDirectionFactor : 1.0;
	//最后的光的效果是 （漫射光的颜色*漫射光的影响系数*漫射光的具体光强系数）
	return vec4(light.Color * light.DiffuseIntensity * LightDirectionFactor, 1.0f);
};

//镜面光最后的影响计算(光源是点光源)
vec4 CaculateSpecularColor_pointLight(PointLight light, vec3 PointPosition, vec3 NormalInWorld,vec3 EyePosition ,float SpecularPower)
{
	//指向眼睛的方向向量
	vec3 VertexToEye= normalize(EyePosition - PointPosition);
	//入射光线向量（光源指向正在处理的点）
	vec3 LightDirection = PointPosition - light.position;
	//反射光线向量（使用内置的reflect函数计算，第一个参数为入射光线向量，第二个参数为法线向量）
	vec3 LightReflect = normalize(reflect(LightDirection, NormalInWorld));
	
	//光照距离
	float LightDistance = length(LightDirection);

	//镜面反射的具体光强系数
	float SpecularFactor = dot(VertexToEye, LightReflect);

	//如果最后的光强系数大于0，说明眼睛和反射光线在同一方向，则计算对一个的镜面反射光光强，如果小于等于零，说明眼睛不再反射光影响范围内，因此将反射光光强赋为0
	if (SpecularFactor > 0 && SpecularFactor>cos(10)) 
	{
		if (dot(NormalInWorld, VertexToEye) <= 0)
		{
			return vec4(0.0, 0.0, 0.0, 0.0);
		}
		else
		{
			SpecularFactor = pow(SpecularFactor, SpecularPower);
			return vec4(light.Color * light.SpecularIntensity * SpecularFactor, 1.0f) /
				(light.Attenuation_constant + light.Attenuation_liner * LightDistance + light.Attenuation_exp * LightDistance * LightDistance);
			//return vec4(light.Color * light.SpecularIntensity * SpecularFactor, 1.0f);
		}        
    }
	else
	{
		return vec4(0.0,0.0,0.0,0.0);
	}
};

//镜面光最后的影响计算(光源是平行光源)
vec4 CaculateSpecularColor_directionLight(DirectionalLight light, vec3 PointPosition, vec3 NormalInWorld, vec3 EyePosition, float SpecularPower)
{
	//指向眼睛的方向向量
	vec3 VertexToEye = normalize(EyePosition - PointPosition);

	//反射光线向量（使用内置的reflect函数计算，第一个参数为入射光线向量，第二个参数为法线向量）
	vec3 LightReflect = normalize(reflect(light.Direction, NormalInWorld));


	//镜面反射的具体光强系数
	float SpecularFactor = dot(VertexToEye, LightReflect);

	//如果最后的光强系数大于0，说明眼睛和反射光线在同一方向，则计算对一个的镜面反射光光强，如果小于等于零，说明眼睛不再反射光影响范围内，因此将反射光光强赋为0
	if (SpecularFactor > 0)
	{
		SpecularFactor = pow(SpecularFactor, SpecularPower);
		return vec4(light.Color * light.SpecularIntensity * SpecularFactor, 1.0f);
	}
	else
	{
		return vec4(0.0, 0.0, 0.0, 0.0);
	}
};

void main()
{
	
	//位于(0,1.5,0)的红色点光源
	PointLight RedPointLight;
	RedPointLight.Color=vec3(1.0,0.0,0.0);
	RedPointLight.AmbientIntensity=0.2;
	RedPointLight.DiffuseIntensity=0.8;
	RedPointLight.SpecularIntensity= 0.8;
	RedPointLight.Attenuation_constant= 0.2;
	RedPointLight.Attenuation_liner= 0.1;
	RedPointLight.Attenuation_exp= 0.1;
	RedPointLight.position=vec3(0.0,0.0,3.0);

	//位于(-1.5,0,0)的绿色点光源
	PointLight GreenPointLight;
	GreenPointLight.Color=vec3(0.0,1.0,0.0);
	GreenPointLight.AmbientIntensity= 0.2;
	GreenPointLight.DiffuseIntensity= 0.8;
	GreenPointLight.SpecularIntensity= 0.8;
	GreenPointLight.Attenuation_constant= 0.2;
	GreenPointLight.Attenuation_liner= 0.1;
	GreenPointLight.Attenuation_exp= 0.1;
	GreenPointLight.position=vec3(-5.0,0.0,0.0);

	//位于(0,0,-1.5)的蓝色点光源
	PointLight BluePointLight;
	BluePointLight.Color=vec3(0.0,0.0,1.0);
	BluePointLight.AmbientIntensity= 0.2;
	BluePointLight.DiffuseIntensity=0.8;
	BluePointLight.SpecularIntensity=0.8;
	BluePointLight.Attenuation_constant= 0.2;
	BluePointLight.Attenuation_liner= 0.1;
	BluePointLight.Attenuation_exp= 0.1;
	BluePointLight.position=vec3(0.0,6.0,0.0);

	vec4 MatarialColor = vec4(1.0,1.0,1.0,1.0);//定义材质颜色

	//计算环境光光强
	vec4 AmbientColor = vec4(RedPointLight.Color * RedPointLight.AmbientIntensity, 1.0f)+
						vec4(BluePointLight.Color * BluePointLight.AmbientIntensity, 1.0f)+
						vec4(GreenPointLight.Color * GreenPointLight.AmbientIntensity, 1.0f); 

	//计算漫射光强
	vec4 DiffuseColor1 = CaculateDiffuseColor_pointLight(RedPointLight, PointPosition, NormalInWorld);
	vec4 DiffuseColor2 = CaculateDiffuseColor_pointLight(GreenPointLight, PointPosition, NormalInWorld);
	vec4 DiffuseColor3 = CaculateDiffuseColor_pointLight(BluePointLight, PointPosition, NormalInWorld);
	
	//计算镜面反射光
	vec4 SpecularColor1 = CaculateSpecularColor_pointLight(RedPointLight, PointPosition, NormalInWorld,EyePosition,1.0);
	vec4 SpecularColor2 = CaculateSpecularColor_pointLight(GreenPointLight, PointPosition, NormalInWorld,EyePosition,1.0);
	vec4 SpecularColor3 = CaculateSpecularColor_pointLight(BluePointLight, PointPosition, NormalInWorld,EyePosition,1.0);

	//FragColor = MatarialColor * (
	//		AmbientColor + 
	//		DiffuseColor1 + DiffuseColor2 + DiffuseColor3+
	//		SpecularColor1+ SpecularColor2+ SpecularColor3);

	FragColor = texture2D(gSampler, TextureAfterTrans.st) * (
			AmbientColor + 
			DiffuseColor1 + DiffuseColor2 + DiffuseColor3 +
			SpecularColor1 + SpecularColor2 + SpecularColor3);

	//FragColor = texture2D(gSampler, TextureAfterTrans.st);

}