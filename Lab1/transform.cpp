
#include "transform.h"

const Matrix4f* Transform::GetCameraTrans()
{
	//�������任����
	GetWorldTrans();

	//����ӽǱ任�����Լ�͸�ӱ任����
	Matrix4f CameraTranslationTrans, CameraRotateTrans, PersProjTrans;

	//����ǰtransform��� camera(����ӽǱ任) persProj��͸�ӱ任�����Բ������룬������Ӧ�ı任����
	CameraTranslationTrans.InitTranslationTransform(-m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z);
	CameraRotateTrans.InitCameraTransform(m_camera.Target, m_camera.Up);
	PersProjTrans.InitPersProjTransform(m_persProj.FOV, m_persProj.Width, m_persProj.Height, m_persProj.zNear, m_persProj.zFar);

	m_CameraTransformation = PersProjTrans * CameraRotateTrans * CameraTranslationTrans * m_WorldTransformation; //�õ���������ӽǵı任����
	return &m_CameraTransformation;
}

const Matrix4f* Transform::GetWorldTrans()
{
	//�任����
	Matrix4f ScaleTrans, RotateTrans, TranslationTrans;

	//����ǰtransform��� scale(����) rotate(��ת) translation��ƽ�ƣ� �任�������룬������Ӧ�ı任����
	ScaleTrans.InitScaleTransform(m_scale.x, m_scale.y, m_scale.z);
	RotateTrans.InitRotateTransform(m_rotateInfo.x, m_rotateInfo.y, m_rotateInfo.z);
	TranslationTrans.InitTranslationTransform(m_worldPos.x, m_worldPos.y, m_worldPos.z);

	m_WorldTransformation = TranslationTrans * RotateTrans * ScaleTrans;//�õ����ı任����
	return &m_WorldTransformation;
}




