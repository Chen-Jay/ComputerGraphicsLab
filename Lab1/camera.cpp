
#include <GL/freeglut.h>
#include "camera.h"

const static float STEP_SCALE = 0.05f;	//步幅，用于在移动摄像机时，每一次按键所对应的移动长度
const static int MARGIN = 10;


//默认构造函数，指定相机的默认位置
Camera::Camera(int WindowWidth, int WindowHeight)
{
	//设置窗口大小，保证相机一开始位于屏幕的中央（眼睛坐标系的中央）
    m_windowWidth  = WindowWidth;
    m_windowHeight = WindowHeight;

	//设置相机在世界坐标系的位置、观看方向还有头顶方向
    m_pos          = Vector3f(0.0f, 0.0f, 0.0f);	//相机位置为世界远点
    m_target       = Vector3f(0.0f, 0.0f, 1.0f);	//相机方向
    m_up           = Vector3f(0.0f, 1.0f, 0.0f);	//相机头顶方向

    Init();
}

//参数中传入相机位置
Camera::Camera(int WindowWidth, int WindowHeight, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
{
    m_windowWidth  = WindowWidth;
    m_windowHeight = WindowHeight;
    m_pos = Pos;

    m_target = Target;
    m_target.Normalize(); //单位化

    m_up = Up;
    m_up.Normalize();	//单位化

    Init();
}

//相机初始化
void Camera::Init()
{
	
    Vector3f HTarget(m_target.x, 0.0, m_target.z);
    HTarget.Normalize(); //单位化，这样z轴上的分量就是用于计算水平倾角的角度的sin值，y轴上的分量就是垂直倾角的sin值
    
	//根据向量在不同的象限初始化水平倾角
    if (HTarget.z >= 0.0f)//在一二象限
    {
        if (HTarget.x >= 0.0f)//第一象限
        {
            m_AngleH = 360.0f - ToDegree(asin(HTarget.z));
        }
        else//第二象限
        {
            m_AngleH = 180.0f + ToDegree(asin(HTarget.z));
        }
    }
    else//三四象限
    {
        if (HTarget.x >= 0.0f)//第四象限
        {
            m_AngleH = ToDegree(asin(-HTarget.z));
        }
        else//第三象限
        {
            m_AngleH = 90.0f + ToDegree(asin(-HTarget.z));
        }
    }
    
	//根据y轴上的分量确定垂直分量
    m_AngleV = -ToDegree(asin(m_target.y));

	//将判定是否处于屏幕边缘的变量设为否定
    m_OnUpperEdge = false;
    m_OnLowerEdge = false;
    m_OnLeftEdge  = false;
    m_OnRightEdge = false;

	//设置鼠标的位置参数至屏幕中央
    m_mousePos.x  = m_windowWidth / 2;
    m_mousePos.y  = m_windowHeight / 2;

	//设置鼠标位置
    glutWarpPointer(m_mousePos.x, m_mousePos.y);
}

//如果键盘有输入，则改变相机的空间坐标系位置
bool Camera::OnKeyboard(int Key)
{
    bool Ret = false;

    switch (Key) {

	//摄像机前进,pos分量加上 步幅*面向方向向量
    case GLUT_KEY_UP:
        {
            m_pos += (m_target * STEP_SCALE);
            Ret = true;
        }
        break;
	//摄像机后退 pos分量减去 步幅*面向方向向量
    case GLUT_KEY_DOWN:
        {
            m_pos -= (m_target * STEP_SCALE);
            Ret = true;
        }
        break;
	
	//让up方向分量和target方向做叉积，得到与它们两个组成平面垂直的向量，然后通过更改叉积的顺序来设置时左还是右

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

//如果鼠标移动，则更新相机方向
void Camera::OnMouse(int x, int y)
{
	//鼠标在屏幕上变化的x,y分量
    const int DeltaX = x - m_mousePos.x;
    const int DeltaY = y - m_mousePos.y;

    m_mousePos.x = x;
    m_mousePos.y = y;

    m_AngleH += (float)DeltaX / 20.0f;	//根据x的改变量来改变水平倾角（其中20.0相当于鼠标的灵敏度，即改变单位长度的坐标，对应多少的角度）

    m_AngleV += (float)DeltaY / 20.0f;	//根据y的改变量来改变垂直倾角

	//考虑是否到达水平方向的屏幕边缘，如果是，则把对应状态设置为TRUE，则会在渲染时自动改变方向
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

//如果发现鼠标在屏幕边缘，则自动更新相机方向
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


//将水平倾角和垂直倾角的改变作用在相机上，反映在相机的target和up的改变上
void Camera::Update()
{
    const Vector3f Vaxis(0.0f, 1.0f, 0.0f); //y轴正方向

    // Rotate the view vector by the horizontal angle around the vertical axis
	//改变相机的水平倾角
    Vector3f View(1.0f, 0.0f, 0.0f);	//x轴正方向
    View.Rotate(m_AngleH, Vaxis);	//绕y轴旋转水平倾角，得到相机方向
    View.Normalize();	//单位化

    // Rotate the view vector by the vertical angle around the horizontal axis
    Vector3f Haxis = Vaxis.Cross(View); //算出U方向
    Haxis.Normalize();	//单位化
    View.Rotate(m_AngleV, Haxis);	//根据垂直倾角绕U方向进行相机方向旋转
       
    m_target = View;
    m_target.Normalize();

    m_up = m_target.Cross(Haxis);
    m_up.Normalize();
}
