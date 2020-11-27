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

/**
 * Structure of the PointAt Matrix:
 * | Ax | Ay | Az | 0 |
 * | Bx | By | Bz | 0 |
 * | Cx | Cy | Cz | 0 |
 * | Tx | Ty | Tz | 1 |
 */
math::Matrix4x4<float> PointAt(const math::Vec3<float> &position, const math::Vec3<float> &target, const math::Vec3<float> &up)
{
	// Vector from the position to the target is the new forward direction
	math::Vec3<float> forwardUnit = math::SubtractVectors(target, position);
	forwardUnit = math::UnitVector(forwardUnit);

	// Calculate the new up direction of the new forward direction
	float newUpScalar = DotProduct(up, forwardUnit);
	math::Vec3<float> newUpTemp = MultiplyVectorByScalar(forwardUnit, newUpScalar);
	math::Vec3<float> upUnit = SubtractVectors(up, newUpTemp);
	upUnit = math::UnitVector(upUnit);

	// Calculate the new right direction for the new up & forward directions
	math::Vec3<float> rightUnit = CrossProduct(upUnit, forwardUnit);

	// Construct the new transformation matrix
	math::Matrix4x4<float> pointAt;
	pointAt.m[0][0] = rightUnit.x;		pointAt.m[0][1] = rightUnit.y;		pointAt.m[0][2] = rightUnit.z;		pointAt.m[0][3] = 0.0f;
	pointAt.m[1][0] = upUnit.x;			pointAt.m[1][1] = upUnit.y;			pointAt.m[1][2] = rightUnit.z;		pointAt.m[1][3] = 0.0f;
	pointAt.m[2][0] = forwardUnit.x;	pointAt.m[2][1] = forwardUnit.y;	pointAt.m[2][2] = forwardUnit.z;	pointAt.m[2][3] = 0.0f;
	pointAt.m[3][0] = position.x;		pointAt.m[3][1] = position.y;		pointAt.m[3][2] = position.z;		pointAt.m[3][3] = 1.0f;
	return pointAt;
}

/**
 * Structure of the LookAt Matrix:
 * |  Ax  |  Bx  |  Cx  | 0 |
 * |  Ay  |  By  |  Cy  | 0 |
 * |  Az  |  Bz  |  Cz  | 0 |
 * | -T.A | -T.B | -T.C | 1 |
 */
math::Matrix4x4<float> LookAt(math::Matrix4x4<float> const &pointAt)
{
	float tDotA = (pointAt.m[3][0] * pointAt.m[0][0]) + (pointAt.m[3][1] * pointAt.m[0][1]) + (pointAt.m[3][2] * pointAt.m[0][2]);
	float tDotB = (pointAt.m[3][0] * pointAt.m[1][0]) + (pointAt.m[3][1] * pointAt.m[1][1]) + (pointAt.m[3][2] * pointAt.m[1][2]);
	float tDotC = (pointAt.m[3][0] * pointAt.m[2][0]) + (pointAt.m[3][1] * pointAt.m[2][1]) + (pointAt.m[3][2] * pointAt.m[2][2]);

	math::Matrix4x4<float> lookAt;
	lookAt.m[0][0] = pointAt.m[0][0];	lookAt.m[0][1] = pointAt.m[1][0];	lookAt.m[0][2] = pointAt.m[2][0];	lookAt.m[0][3] = 0.0f;
	lookAt.m[1][0] = pointAt.m[0][1];	lookAt.m[1][1] = pointAt.m[1][1];	lookAt.m[1][2] = pointAt.m[2][1];	lookAt.m[1][3] = 0.0f;
	lookAt.m[2][0] = pointAt.m[0][2];	lookAt.m[2][1] = pointAt.m[1][2];	lookAt.m[2][2] = pointAt.m[2][2];	lookAt.m[2][3] = 0.0f;
	lookAt.m[3][0] = -tDotA;			lookAt.m[3][1] = -tDotB;			lookAt.m[3][2] = -tDotC;			lookAt.m[3][3] = 1.0f;
	return lookAt;
}