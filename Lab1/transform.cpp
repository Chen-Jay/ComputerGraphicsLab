
#include "transform.h"

const Matrix4f* Transform::GetCameraTrans()
{
	//获得世界变换矩阵
	GetWorldTrans();

	//相机视角变换矩阵以及透视变换矩阵
	Matrix4f CameraTranslationTrans, CameraRotateTrans, PersProjTrans;

	//将当前transform类的 camera(相机视角变换) persProj（透视变换）属性参数读入，产生相应的变换矩阵
	CameraTranslationTrans.InitTranslationTransform(-m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z);
	CameraRotateTrans.InitCameraTransform(m_camera.Target, m_camera.Up);
	PersProjTrans.InitPersProjTransform(m_persProj.FOV, m_persProj.Width, m_persProj.Height, m_persProj.zNear, m_persProj.zFar);

	m_CameraTransformation = PersProjTrans * CameraRotateTrans * CameraTranslationTrans * m_WorldTransformation; //得到最后的相机视角的变换矩阵
	return &m_CameraTransformation;
}

const Matrix4f* Transform::GetWorldTrans()
{
	//变换矩阵
	Matrix4f ScaleTrans, RotateTrans, TranslationTrans;

	//将当前transform类的 scale(缩放) rotate(旋转) translation（平移） 变换参数读入，产生相应的变换矩阵
	ScaleTrans.InitScaleTransform(m_scale.x, m_scale.y, m_scale.z);
	RotateTrans.InitRotateTransform(m_rotateInfo.x, m_rotateInfo.y, m_rotateInfo.z);
	TranslationTrans.InitTranslationTransform(m_worldPos.x, m_worldPos.y, m_worldPos.z);

	m_WorldTransformation = TranslationTrans * RotateTrans * ScaleTrans;//得到最后的变换矩阵
	return &m_WorldTransformation;
}




