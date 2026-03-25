#pragma once
#include "Vector3.h"

struct Matrix44
{
	float m00, m01, m02, m03;
	float m10, m11, m12, m13;
	float m20, m21, m22, m23;
	float m30, m31, m32, m33;
};

struct Matrix34
{
	float m11, m12, m13, x;
	float m21, m22, m23, y;
	float m31, m32, m33, z;

public:
	const Vector3 GetPosition() const
	{
		Vector3 Return;
		Return.x = x;
		Return.y = y;
		Return.z = z;
		return Return;
	}
};
