#include "math.hpp"
#include <math.h>

math::Matrix4x4<float> MakeProjectionMatrix(float fieldOfVewDeg, float aspectRatio, float nearPlane, float farPlane)
{
	float inverseTangent = 1.0f / std::tanf(fieldOfVewDeg * 0.5f * 3.14159f / 180.0f);

	math::Matrix4x4<float> matrix;
	matrix.m[0][0] = aspectRatio * inverseTangent;
	matrix.m[1][1] = inverseTangent;
	matrix.m[2][2] = farPlane / (farPlane - nearPlane);
	matrix.m[3][2] = (-farPlane * nearPlane) / (farPlane - nearPlane);
	matrix.m[2][3] = 1.0f;
	matrix.m[3][3] = 0.0f;

	return matrix;
}

math::Matrix4x4<float> MakeIdentityMatrix()
{
	math::Matrix4x4<float> matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	return matrix;
}

void SetZAxisRotationMatrix(float theta, math::Matrix4x4<float> &matrix)
{
	float cos = std::cosf(theta);
	float sin = std::sinf(theta);
	matrix.m[0][0] = cos;
	matrix.m[0][1] = -sin;
	matrix.m[1][0] = sin;
	matrix.m[1][1] = cos;
}

math::Matrix4x4<float> MakeZAxisRotationMatrix(float theta)
{
	math::Matrix4x4<float> matrix = MakeIdentityMatrix();
	SetZAxisRotationMatrix(theta, matrix);
	return matrix;
}

void SetXAxisRotationMatrix(float theta, math::Matrix4x4<float> &matrix)
{
	float cos = std::cosf(theta);
	float sin = std::sinf(theta);
	matrix.m[1][1] = cos;
	matrix.m[1][2] = -sin;
	matrix.m[2][1] = sin;
	matrix.m[2][2] = cos;
}

math::Matrix4x4<float> MakeXAxisRotationMatrix(float theta)
{
	math::Matrix4x4<float> matrix = MakeIdentityMatrix();
	SetXAxisRotationMatrix(theta, matrix);
	return matrix;
}

math::Matrix4x4<float> MakeTranslationMatrix(float dispX, float dispY, float dispZ)
{
	math::Matrix4x4<float> matrix = MakeIdentityMatrix();
	matrix.m[3][0] = dispX;
	matrix.m[3][1] = dispY;
	matrix.m[3][2] = dispZ;
	return matrix;
}
