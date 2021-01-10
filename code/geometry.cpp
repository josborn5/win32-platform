#include "math.hpp"
#include <math.h>
#include "geometry.hpp"

namespace gentle
{
	Matrix4x4<float> MakeProjectionMatrix(float fieldOfVewDeg, float aspectRatio, float nearPlane, float farPlane)
	{
		float inverseTangent = 1.0f / std::tanf(fieldOfVewDeg * 0.5f * 3.14159f / 180.0f);

		Matrix4x4<float> matrix;
		matrix.m[0][0] = aspectRatio * inverseTangent;
		matrix.m[1][1] = inverseTangent;
		matrix.m[2][2] = farPlane / (farPlane - nearPlane);
		matrix.m[3][2] = (-farPlane * nearPlane) / (farPlane - nearPlane);
		matrix.m[2][3] = 1.0f;
		matrix.m[3][3] = 0.0f;

		return matrix;
	}

	void SetZAxisRotationMatrix(float theta, Matrix4x4<float> &matrix)
	{
		float cos = std::cosf(theta);
		float sin = std::sinf(theta);
		matrix.m[0][0] = cos;
		matrix.m[0][1] = -sin;
		matrix.m[1][0] = sin;
		matrix.m[1][1] = cos;
	}

	Matrix4x4<float> MakeZAxisRotationMatrix(float theta)
	{
		Matrix4x4<float> matrix = MakeIdentityMatrix<float>();
		SetZAxisRotationMatrix(theta, matrix);
		return matrix;
	}

	void SetYAxisRotationMatrix(float theta, Matrix4x4<float> &matrix)
	{
		float cos = std::cosf(theta);
		float sin = std::sinf(theta);
		matrix.m[0][0] = cos;
		matrix.m[0][2] = sin;
		matrix.m[2][0] = -sin;
		matrix.m[2][2] = cos;
	}

	Matrix4x4<float> MakeYAxisRotationMatrix(float theta)
	{
		Matrix4x4<float> matrix = MakeIdentityMatrix<float>();
		SetYAxisRotationMatrix(theta, matrix);
		return matrix;
	}

	void SetXAxisRotationMatrix(float theta, Matrix4x4<float> &matrix)
	{
		float cos = std::cosf(theta);
		float sin = std::sinf(theta);
		matrix.m[1][1] = cos;
		matrix.m[1][2] = -sin;
		matrix.m[2][1] = sin;
		matrix.m[2][2] = cos;
	}

	Matrix4x4<float> MakeXAxisRotationMatrix(float theta)
	{
		Matrix4x4<float> matrix = MakeIdentityMatrix<float>();
		SetXAxisRotationMatrix(theta, matrix);
		return matrix;
	}
}