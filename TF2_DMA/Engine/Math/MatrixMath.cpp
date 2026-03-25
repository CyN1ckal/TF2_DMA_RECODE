#include "pch.h"
#include "MatrixMath.h"
#include "Engine/Vector4.h"
#include "TF2/Camera/Camera.h"

bool WorldToScreen(Vector3 WorldLocation, Vector2& Screen)
{
	auto ViewProjectionMatrix = Camera::GetViewProjectionMatrix();

	Vector4 ClipCoords;

	ClipCoords.x = WorldLocation.x * ViewProjectionMatrix.m00 + WorldLocation.y * ViewProjectionMatrix.m01 +
		WorldLocation.z * ViewProjectionMatrix.m02 + ViewProjectionMatrix.m03;

	ClipCoords.y = WorldLocation.x * ViewProjectionMatrix.m10 + WorldLocation.y * ViewProjectionMatrix.m11 +
		WorldLocation.z * ViewProjectionMatrix.m12 + ViewProjectionMatrix.m13;

	ClipCoords.z = WorldLocation.x * ViewProjectionMatrix.m20 + WorldLocation.y * ViewProjectionMatrix.m21 +
		WorldLocation.z * ViewProjectionMatrix.m22 + ViewProjectionMatrix.m23;

	ClipCoords.w = WorldLocation.x * ViewProjectionMatrix.m30 + WorldLocation.y * ViewProjectionMatrix.m31 +
		WorldLocation.z * ViewProjectionMatrix.m32 + ViewProjectionMatrix.m33;

	if (ClipCoords.w < 0.1f)
		return false;

	Vector3 NDC;
	NDC.x = ClipCoords.x / ClipCoords.w;
	NDC.y = ClipCoords.y / ClipCoords.w;
	NDC.z = ClipCoords.z / ClipCoords.w;

	auto GameSize = TF2::GetGameSize();

	float Intermediate = (NDC.x + 1) / 2;
	Intermediate *= GameSize.GameWidth;
	Screen.x = Intermediate;

	Screen.y = -(GameSize.GameHeight / 2 * NDC.y) + (NDC.y + GameSize.GameHeight / 2);

	return 1;
}
