
#include <GL/freeglut.h>
#include "camera.h"

const static float STEP_SCALE = 0.05f;	//�������������ƶ������ʱ��ÿһ�ΰ�������Ӧ���ƶ�����
const static int MARGIN = 10;


//Ĭ�Ϲ��캯����ָ�������Ĭ��λ��
Camera::Camera(int WindowWidth, int WindowHeight)
{
	//���ô��ڴ�С����֤���һ��ʼλ����Ļ�����루�۾�����ϵ�����룩
    m_windowWidth  = WindowWidth;
    m_windowHeight = WindowHeight;

	//�����������������ϵ��λ�á��ۿ�������ͷ������
    m_pos          = Vector3f(0.0f, 0.0f, 0.0f);	//���λ��Ϊ����Զ��
    m_target       = Vector3f(0.0f, 0.0f, 1.0f);	//�������
    m_up           = Vector3f(0.0f, 1.0f, 0.0f);	//���ͷ������

    Init();
}

//�����д������λ��
Camera::Camera(int WindowWidth, int WindowHeight, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
{
    m_windowWidth  = WindowWidth;
    m_windowHeight = WindowHeight;
    m_pos = Pos;

    m_target = Target;
    m_target.Normalize(); //��λ��

    m_up = Up;
    m_up.Normalize();	//��λ��

    Init();
}

//�����ʼ��
void Camera::Init()
{
	
    Vector3f HTarget(m_target.x, 0.0, m_target.z);
    HTarget.Normalize(); //��λ��������z���ϵķ����������ڼ���ˮƽ��ǵĽǶȵ�sinֵ��y���ϵķ������Ǵ�ֱ��ǵ�sinֵ
    
	//���������ڲ�ͬ�����޳�ʼ��ˮƽ���
    if (HTarget.z >= 0.0f)//��һ������
    {
        if (HTarget.x >= 0.0f)//��һ����
        {
            m_AngleH = 360.0f - ToDegree(asin(HTarget.z));
        }
        else//�ڶ�����
        {
            m_AngleH = 180.0f + ToDegree(asin(HTarget.z));
        }
    }
    else//��������
    {
        if (HTarget.x >= 0.0f)//��������
        {
            m_AngleH = ToDegree(asin(-HTarget.z));
        }
        else//��������
        {
            m_AngleH = 90.0f + ToDegree(asin(-HTarget.z));
        }
    }
    
	//����y���ϵķ���ȷ����ֱ����
    m_AngleV = -ToDegree(asin(m_target.y));

	//���ж��Ƿ�����Ļ��Ե�ı�����Ϊ��
    m_OnUpperEdge = false;
    m_OnLowerEdge = false;
    m_OnLeftEdge  = false;
    m_OnRightEdge = false;

	//��������λ�ò�������Ļ����
    m_mousePos.x  = m_windowWidth / 2;
    m_mousePos.y  = m_windowHeight / 2;

	//�������λ��
    glutWarpPointer(m_mousePos.x, m_mousePos.y);
}

//������������룬��ı�����Ŀռ�����ϵλ��
bool Camera::OnKeyboard(int Key)
{
    bool Ret = false;

    switch (Key) {

	//�����ǰ��,pos�������� ����*����������
    case GLUT_KEY_UP:
        {
            m_pos += (m_target * STEP_SCALE);
            Ret = true;
        }
        break;
	//��������� pos������ȥ ����*����������
    case GLUT_KEY_DOWN:
        {
            m_pos -= (m_target * STEP_SCALE);
            Ret = true;
        }
        break;
	
	//��up���������target������������õ��������������ƽ�洹ֱ��������Ȼ��ͨ�����Ĳ����˳��������ʱ������

    case GLUT_KEY_LEFT:
        {
            Vector3f Left = m_target.Cross(m_up);
            Left.Normalize();
            Left *= STEP_SCALE;
            m_pos += Left;
            Ret = true;
        }
        break;

    case GLUT_KEY_RIGHT:
        {
            Vector3f Right = m_up.Cross(m_target);
            Right.Normalize();
            Right *= STEP_SCALE;
            m_pos += Right;
            Ret = true;
        }
        break;
    }

    return Ret;
}

//�������ƶ���������������
void Camera::OnMouse(int x, int y)
{
	//�������Ļ�ϱ仯��x,y����
    const int DeltaX = x - m_mousePos.x;
    const int DeltaY = y - m_mousePos.y;

    m_mousePos.x = x;
    m_mousePos.y = y;

    m_AngleH += (float)DeltaX / 20.0f;	//����x�ĸı������ı�ˮƽ��ǣ�����20.0�൱�����������ȣ����ı䵥λ���ȵ����꣬��Ӧ���ٵĽǶȣ�

    m_AngleV += (float)DeltaY / 20.0f;	//����y�ĸı������ı䴹ֱ���

	//�����Ƿ񵽴�ˮƽ�������Ļ��Ե������ǣ���Ѷ�Ӧ״̬����ΪTRUE���������Ⱦʱ�Զ��ı䷽��
    if (DeltaX == 0) {
        if (x <= MARGIN) {
        //    m_AngleH -= 1.0f;
            m_OnLeftEdge = true;
        }
        else if (x >= (m_windowWidth - MARGIN)) {
        //    m_AngleH += 1.0f;
            m_OnRightEdge = true;
        }
    }
    else {
        m_OnLeftEdge = false;
        m_OnRightEdge = false;
    }

    if (DeltaY == 0) {
        if (y <= MARGIN) {
            m_OnUpperEdge = true;
        }
        else if (y >= (m_windowHeight - MARGIN)) {
            m_OnLowerEdge = true;
        }
    }
    else {
        m_OnUpperEdge = false;
        m_OnLowerEdge = false;
    }

    Update();
}

//��������������Ļ��Ե�����Զ������������
void Camera::OnRender()
{
    bool ShouldUpdate = false;

    if (m_OnLeftEdge) {
        m_AngleH -= 0.1f;
        ShouldUpdate = true;
    }
    else if (m_OnRightEdge) {
        m_AngleH += 0.1f;
        ShouldUpdate = true;
    }

    if (m_OnUpperEdge) {
        if (m_AngleV > -90.0f) {
            m_AngleV -= 0.1f;
            ShouldUpdate = true;
        }
    }
    else if (m_OnLowerEdge) {
        if (m_AngleV < 90.0f) {
           m_AngleV += 0.1f;
           ShouldUpdate = true;
        }
    }

    if (ShouldUpdate) {
        Update();
    }
}


//��ˮƽ��Ǻʹ�ֱ��ǵĸı�����������ϣ���ӳ�������target��up�ĸı���
void Camera::Update()
{
    const Vector3f Vaxis(0.0f, 1.0f, 0.0f); //y��������

    // Rotate the view vector by the horizontal angle around the vertical axis
	//�ı������ˮƽ���
    Vector3f View(1.0f, 0.0f, 0.0f);	//x��������
    View.Rotate(m_AngleH, Vaxis);	//��y����תˮƽ��ǣ��õ��������
    View.Normalize();	//��λ��

    // Rotate the view vector by the vertical angle around the horizontal axis
    Vector3f Haxis = Vaxis.Cross(View); //���U����
    Haxis.Normalize();	//��λ��
    View.Rotate(m_AngleV, Haxis);	//���ݴ�ֱ�����U����������������ת
       
    m_target = View;
    m_target.Normalize();

    m_up = m_target.Cross(Haxis);
    m_up.Normalize();
}
