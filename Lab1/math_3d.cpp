#include "math_3d.h"

Vector3f Vector3f::Cross(const Vector3f& v) const
{
	const float _x = y * v.z - z * v.y;
	const float _y = z * v.x - x * v.z;
	const float _z = x * v.y - y * v.x;

	return Vector3f(_x, _y, _z);
}

Vector3f& Vector3f::Normalize()
{
	const float Length = sqrtf(x * x + y * y + z * z);

	x /= Length;
	y /= Length;
	z /= Length;

	return *this;
}

void Vector3f::Rotate(float Angle, const Vector3f& Axe)
{
	const float SinHalfAngle = sinf(ToRadian(Angle / 2));
	const float CosHalfAngle = cosf(ToRadian(Angle / 2));

	const float Rx = Axe.x * SinHalfAngle;
	const float Ry = Axe.y * SinHalfAngle;
	const float Rz = Axe.z * SinHalfAngle;
	const float Rw = CosHalfAngle;
	Quaternion RotationQ(Rx, Ry, Rz, Rw);

	Quaternion ConjugateQ = RotationQ.Conjugate();
	//  ConjugateQ.Normalize();
	Quaternion W = RotationQ * (*this) * ConjugateQ;

	x = W.x;
	y = W.y;
	z = W.z;
}


void Matrix4f::InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ)
{
	matrix[0][0] = ScaleX; matrix[0][1] = 0.0f;   matrix[0][2] = 0.0f;   matrix[0][3] = 0.0f;
	matrix[1][0] = 0.0f;   matrix[1][1] = ScaleY; matrix[1][2] = 0.0f;   matrix[1][3] = 0.0f;
	matrix[2][0] = 0.0f;   matrix[2][1] = 0.0f;   matrix[2][2] = ScaleZ; matrix[2][3] = 0.0f;
	matrix[3][0] = 0.0f;   matrix[3][1] = 0.0f;   matrix[3][2] = 0.0f;   matrix[3][3] = 1.0f;
}

void Matrix4f::InitRotateTransform(float RotateX, float RotateY, float RotateZ)
{
	Matrix4f rx, ry, rz;

	const float x = ToRadian(RotateX);
	const float y = ToRadian(RotateY);
	const float z = ToRadian(RotateZ);

	rx.matrix[0][0] = 1.0f; rx.matrix[0][1] = 0.0f; rx.matrix[0][2] = 0.0f; rx.matrix[0][3] = 0.0f;
	rx.matrix[1][0] = 0.0f; rx.matrix[1][1] = cosf(x); rx.matrix[1][2] = -sinf(x); rx.matrix[1][3] = 0.0f;
	rx.matrix[2][0] = 0.0f; rx.matrix[2][1] = sinf(x); rx.matrix[2][2] = cosf(x); rx.matrix[2][3] = 0.0f;
	rx.matrix[3][0] = 0.0f; rx.matrix[3][1] = 0.0f; rx.matrix[3][2] = 0.0f; rx.matrix[3][3] = 1.0f;

	ry.matrix[0][0] = cosf(y); ry.matrix[0][1] = 0.0f; ry.matrix[0][2] = -sinf(y); ry.matrix[0][3] = 0.0f;
	ry.matrix[1][0] = 0.0f; ry.matrix[1][1] = 1.0f; ry.matrix[1][2] = 0.0f; ry.matrix[1][3] = 0.0f;
	ry.matrix[2][0] = sinf(y); ry.matrix[2][1] = 0.0f; ry.matrix[2][2] = cosf(y); ry.matrix[2][3] = 0.0f;
	ry.matrix[3][0] = 0.0f; ry.matrix[3][1] = 0.0f; ry.matrix[3][2] = 0.0f; ry.matrix[3][3] = 1.0f;

	rz.matrix[0][0] = cosf(z); rz.matrix[0][1] = -sinf(z); rz.matrix[0][2] = 0.0f; rz.matrix[0][3] = 0.0f;
	rz.matrix[1][0] = sinf(z); rz.matrix[1][1] = cosf(z); rz.matrix[1][2] = 0.0f; rz.matrix[1][3] = 0.0f;
	rz.matrix[2][0] = 0.0f; rz.matrix[2][1] = 0.0f; rz.matrix[2][2] = 1.0f; rz.matrix[2][3] = 0.0f;
	rz.matrix[3][0] = 0.0f; rz.matrix[3][1] = 0.0f; rz.matrix[3][2] = 0.0f; rz.matrix[3][3] = 1.0f;

	*this = rz * ry * rx;
}

void Matrix4f::InitTranslationTransform(float x, float y, float z)
{
	matrix[0][0] = 1.0f; matrix[0][1] = 0.0f; matrix[0][2] = 0.0f; matrix[0][3] = x;
	matrix[1][0] = 0.0f; matrix[1][1] = 1.0f; matrix[1][2] = 0.0f; matrix[1][3] = y;
	matrix[2][0] = 0.0f; matrix[2][1] = 0.0f; matrix[2][2] = 1.0f; matrix[2][3] = z;
	matrix[3][0] = 0.0f; matrix[3][1] = 0.0f; matrix[3][2] = 0.0f; matrix[3][3] = 1.0f;
}


void Matrix4f::InitCameraTransform(const Vector3f& Target, const Vector3f& Up)
{
	Vector3f N = Target;
	N.Normalize();
	Vector3f U = Up;
	U.Normalize();
	U = U.Cross(N);
	Vector3f V = N.Cross(U);

	matrix[0][0] = U.x;   matrix[0][1] = U.y;   matrix[0][2] = U.z;   matrix[0][3] = 0.0f;
	matrix[1][0] = V.x;   matrix[1][1] = V.y;   matrix[1][2] = V.z;   matrix[1][3] = 0.0f;
	matrix[2][0] = N.x;   matrix[2][1] = N.y;   matrix[2][2] = N.z;   matrix[2][3] = 0.0f;
	matrix[3][0] = 0.0f;  matrix[3][1] = 0.0f;  matrix[3][2] = 0.0f;  matrix[3][3] = 1.0f;
}

void Matrix4f::InitPersProjTransform(float FOV, float Width, float Height, float zNear, float zFar)
{
	const float ar = Width / Height;
	const float zRange = zNear - zFar;
	const float tanHalfFOV = tanf(ToRadian(FOV / 2.0f));

	matrix[0][0] = 1.0f / (tanHalfFOV * ar); matrix[0][1] = 0.0f;            matrix[0][2] = 0.0f;          matrix[0][3] = 0.0;
	matrix[1][0] = 0.0f;                   matrix[1][1] = 1.0f / tanHalfFOV; matrix[1][2] = 0.0f;          matrix[1][3] = 0.0;
	matrix[2][0] = 0.0f;                   matrix[2][1] = 0.0f;            matrix[2][2] = (-zNear - zFar) / zRange; matrix[2][3] = 2.0f * zFar*zNear / zRange;
	matrix[3][0] = 0.0f;                   matrix[3][1] = 0.0f;            matrix[3][2] = 1.0f;          matrix[3][3] = 0.0;
}


Quaternion::Quaternion(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

void Quaternion::Normalize()
{
	float Length = sqrtf(x * x + y * y + z * z + w * w);

	x /= Length;
	y /= Length;
	z /= Length;
	w /= Length;
}


Quaternion Quaternion::Conjugate()
{
	Quaternion ret(-x, -y, -z, w);
	return ret;
}

Quaternion operator*(const Quaternion& l, const Quaternion& r)
{
	const float w = (l.w * r.w) - (l.x * r.x) - (l.y * r.y) - (l.z * r.z);
	const float x = (l.x * r.w) + (l.w * r.x) + (l.y * r.z) - (l.z * r.y);
	const float y = (l.y * r.w) + (l.w * r.y) + (l.z * r.x) - (l.x * r.z);
	const float z = (l.z * r.w) + (l.w * r.z) + (l.x * r.y) - (l.y * r.x);

	Quaternion ret(x, y, z, w);

	return ret;
}

Quaternion operator*(const Quaternion& q, const Vector3f& v)
{
	const float w = -(q.x * v.x) - (q.y * v.y) - (q.z * v.z);
	const float x = (q.w * v.x) + (q.y * v.z) - (q.z * v.y);
	const float y = (q.w * v.y) + (q.z * v.x) - (q.x * v.z);
	const float z = (q.w * v.z) + (q.x * v.y) - (q.y * v.x);

	Quaternion ret(x, y, z, w);

	return ret;
}