#version 330

//in vec4 Color;

in vec2 TextureAfterTrans;	//����ľ�����ֵ����������

//�������
in vec3 PointPosition; //�������������ϵ�����������
in vec3 NormalInWorld; //�������������ϵ������ķ�������
in vec3 EyePosition; //�������������ϵ���۾�������

out vec4 FragColor;

uniform sampler2D gSampler;	//����Ĳ�����ȫ�ֱ���

//ƽ�й�Դ
struct DirectionalLight
{
	vec3 Color;//���
	float AmbientIntensity; //������ϵ��
	float DiffuseIntensity;//������ǿ
	float SpecularIntensity;//���淴����ǿ
	vec3 Direction;//���շ���
};

//���Դ
struct PointLight
{
	vec3 Color;
	float DiffuseIntensity; //������ϵ��
	float AmbientIntensity; //������ϵ��
	float SpecularIntensity; //�����ϵ��
	float Attenuation_constant; //����˥��ϵ��
	float Attenuation_liner; //����˥��ϵ��
	float Attenuation_exp;  //ָ��˥��ϵ��
	vec3 position; //���Դλ��
};

//���������Ч������(��Դ�ǵ��Դ)
vec4 CaculateDiffuseColor_pointLight(PointLight light, vec3 PointPosition, vec3 NormalInWorld)
{
	//��������������ӹ�Դָ��ǰ���������ڴ���ĵ㣩(��ʱ�����е�λ������Ϊ��Ҫ����������룩
	vec3 LightDirection = PointPosition - light.position;
	//�õ���Դ�������ϵĵ�ľ���
	float LightDistance = length(LightDirection);
	//������ڴ���ĵ�ķ��ߺ͹��շ���õ������ľ����ǿ����ʱ��Ҫ���е�λ������Ϊ����������нǣ�
	float LightDirectionFactor =dot(normalize(NormalInWorld), normalize(-LightDirection));
	//�ж��õ��ľ����ǿϵ�������С��0����˵�������Ǵ����屳�洫���ģ���˽�����ϵ����Ϊ0���������һ(�����ϲ��ᣬ��������Ϊ1��
	LightDirectionFactor = LightDirectionFactor>0.0?LightDirectionFactor:0.0;
	LightDirectionFactor = LightDirectionFactor<1.0?LightDirectionFactor:1.0;
	//���Ĺ��Ч���� ����������ɫ*������Ӱ��ϵ��*�����ľ����ǿϵ����/ �����Դ�ĳ���˥��ϵ��+���վ���*���Դ������˥��ϵ��+���վ����ƽ��*���Դ��ָ��˥��ϵ����
	return vec4(light.Color * light.DiffuseIntensity * LightDirectionFactor, 1.0f)/
						(light.Attenuation_constant + light.Attenuation_liner * LightDistance + light.Attenuation_exp * LightDistance * LightDistance);
	//return vec4(light.Color * light.DiffuseIntensity * LightDirectionFactor, 1.0f);
};

//���������Ч������(��Դ��ƽ�й�Դ)
vec4 CaculateDiffuseColor_directionLight(DirectionalLight light, vec3 PointPosition, vec3 NormalInWorld)
{
	//������ڴ���ĵ�ķ��ߺ͹��շ���õ������ľ����ǿ
	float LightDirectionFactor = dot(normalize(NormalInWorld), normalize(light.Direction));
	//�ж��õ��ľ����ǿϵ�������С��0����˵�������Ǵ����屳�洫���ģ���˽�����ϵ����Ϊ0���������һ(�����ϲ��ᣬ��������Ϊ1��
	LightDirectionFactor = LightDirectionFactor > 0.0 ? LightDirectionFactor : 0.0;
	LightDirectionFactor = LightDirectionFactor < 1.0 ? LightDirectionFactor : 1.0;
	//���Ĺ��Ч���� ����������ɫ*������Ӱ��ϵ��*�����ľ����ǿϵ����
	return vec4(light.Color * light.DiffuseIntensity * LightDirectionFactor, 1.0f);
};

//���������Ӱ�����(��Դ�ǵ��Դ)
vec4 CaculateSpecularColor_pointLight(PointLight light, vec3 PointPosition, vec3 NormalInWorld,vec3 EyePosition ,float SpecularPower)
{
	//ָ���۾��ķ�������
	vec3 VertexToEye= normalize(EyePosition - PointPosition);
	//���������������Դָ�����ڴ���ĵ㣩
	vec3 LightDirection = PointPosition - light.position;
	//�������������ʹ�����õ�reflect�������㣬��һ������Ϊ��������������ڶ�������Ϊ����������
	vec3 LightReflect = normalize(reflect(LightDirection, NormalInWorld));
	
	//���վ���
	float LightDistance = length(LightDirection);

	//���淴��ľ����ǿϵ��
	float SpecularFactor = dot(VertexToEye, LightReflect);

	//������Ĺ�ǿϵ������0��˵���۾��ͷ��������ͬһ����������һ���ľ��淴����ǿ�����С�ڵ����㣬˵���۾����ٷ����Ӱ�췶Χ�ڣ���˽�������ǿ��Ϊ0
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

//���������Ӱ�����(��Դ��ƽ�й�Դ)
vec4 CaculateSpecularColor_directionLight(DirectionalLight light, vec3 PointPosition, vec3 NormalInWorld, vec3 EyePosition, float SpecularPower)
{
	//ָ���۾��ķ�������
	vec3 VertexToEye = normalize(EyePosition - PointPosition);

	//�������������ʹ�����õ�reflect�������㣬��һ������Ϊ��������������ڶ�������Ϊ����������
	vec3 LightReflect = normalize(reflect(light.Direction, NormalInWorld));


	//���淴��ľ����ǿϵ��
	float SpecularFactor = dot(VertexToEye, LightReflect);

	//������Ĺ�ǿϵ������0��˵���۾��ͷ��������ͬһ����������һ���ľ��淴����ǿ�����С�ڵ����㣬˵���۾����ٷ����Ӱ�췶Χ�ڣ���˽�������ǿ��Ϊ0
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
	
	//λ��(0,1.5,0)�ĺ�ɫ���Դ
	PointLight RedPointLight;
	RedPointLight.Color=vec3(1.0,0.0,0.0);
	RedPointLight.AmbientIntensity=0.2;
	RedPointLight.DiffuseIntensity=0.8;
	RedPointLight.SpecularIntensity= 0.8;
	RedPointLight.Attenuation_constant= 0.2;
	RedPointLight.Attenuation_liner= 0.1;
	RedPointLight.Attenuation_exp= 0.1;
	RedPointLight.position=vec3(0.0,0.0,3.0);

	//λ��(-1.5,0,0)����ɫ���Դ
	PointLight GreenPointLight;
	GreenPointLight.Color=vec3(0.0,1.0,0.0);
	GreenPointLight.AmbientIntensity= 0.2;
	GreenPointLight.DiffuseIntensity= 0.8;
	GreenPointLight.SpecularIntensity= 0.8;
	GreenPointLight.Attenuation_constant= 0.2;
	GreenPointLight.Attenuation_liner= 0.1;
	GreenPointLight.Attenuation_exp= 0.1;
	GreenPointLight.position=vec3(-5.0,0.0,0.0);

	//λ��(0,0,-1.5)����ɫ���Դ
	PointLight BluePointLight;
	BluePointLight.Color=vec3(0.0,0.0,1.0);
	BluePointLight.AmbientIntensity= 0.2;
	BluePointLight.DiffuseIntensity=0.8;
	BluePointLight.SpecularIntensity=0.8;
	BluePointLight.Attenuation_constant= 0.2;
	BluePointLight.Attenuation_liner= 0.1;
	BluePointLight.Attenuation_exp= 0.1;
	BluePointLight.position=vec3(0.0,6.0,0.0);

	vec4 MatarialColor = vec4(1.0,1.0,1.0,1.0);//���������ɫ

	//���㻷�����ǿ
	vec4 AmbientColor = vec4(RedPointLight.Color * RedPointLight.AmbientIntensity, 1.0f)+
						vec4(BluePointLight.Color * BluePointLight.AmbientIntensity, 1.0f)+
						vec4(GreenPointLight.Color * GreenPointLight.AmbientIntensity, 1.0f); 

	//���������ǿ
	vec4 DiffuseColor1 = CaculateDiffuseColor_pointLight(RedPointLight, PointPosition, NormalInWorld);
	vec4 DiffuseColor2 = CaculateDiffuseColor_pointLight(GreenPointLight, PointPosition, NormalInWorld);
	vec4 DiffuseColor3 = CaculateDiffuseColor_pointLight(BluePointLight, PointPosition, NormalInWorld);
	
	//���㾵�淴���
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