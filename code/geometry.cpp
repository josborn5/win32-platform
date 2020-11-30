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

void SetYAxisRotationMatrix(float theta, math::Matrix4x4<float> &matrix)
{
	float cos = std::cosf(theta);
	float sin = std::sinf(theta);
	matrix.m[0][0] = cos;
	matrix.m[0][2] = sin;
	matrix.m[2][0] = -sin;
	matrix.m[2][2] = cos;
}

math::Matrix4x4<float> MakeYAxisRotationMatrix(float theta)
{
	math::Matrix4x4<float> matrix = MakeIdentityMatrix();
	SetYAxisRotationMatrix(theta, matrix);
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
math::Matrix4x4<float> PointAt(const math::Vec4<float> &position, const math::Vec4<float> &target, const math::Vec4<float> &up)
{
	// Vector from the position to the target is the new forward direction
	math::Vec4<float> forwardUnit = math::SubtractVectors(target, position);
	forwardUnit = math::UnitVector(forwardUnit);

	// Calculate the new up direction of the new forward direction
	float newUpScalar = DotProduct(up, forwardUnit);
	math::Vec4<float> newUpTemp = MultiplyVectorByScalar(forwardUnit, newUpScalar);
	math::Vec4<float> upUnit = SubtractVectors(up, newUpTemp);
	upUnit = math::UnitVector(upUnit);

	// Calculate the new right direction for the new up & forward directions
	math::Vec4<float> rightUnit = CrossProduct(upUnit, forwardUnit);

	// Construct the new transformation matrix
	math::Matrix4x4<float> pointAt;
	pointAt.m[0][0] = rightUnit.x;		pointAt.m[0][1] = rightUnit.y;		pointAt.m[0][2] = rightUnit.z;		pointAt.m[0][3] = 0.0f;
	pointAt.m[1][0] = upUnit.x;			pointAt.m[1][1] = upUnit.y;			pointAt.m[1][2] = upUnit.z;			pointAt.m[1][3] = 0.0f;
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

math::Vec4<float> IntersectPlane(const math::Vec4<float> &planeP, const math::Vec4<float> &planeN, const math::Vec4<float> &lineStart, const math::Vec4<float> lineEnd)
{
	math::Vec4<float> normalizedPlaneN = math::UnitVector(planeN);
	float planeD = DotProduct(normalizedPlaneN, planeP);
	float ad = DotProduct(lineStart, normalizedPlaneN);
	float bd = DotProduct(lineEnd, normalizedPlaneN);
	float t = (planeD - ad) / (bd - ad);
	math::Vec4<float> lineStartToEnd = SubtractVectors(lineEnd, lineStart);
	math::Vec4<float> lineToIntersect = MultiplyVectorByScalar(lineStartToEnd, t);
	return AddVectors(lineStart, lineToIntersect);
}

float ShortestDistanceFromPointToPlane(const math::Vec4<float> &point, const math::Vec4<float> &planeP, const math::Vec4<float> &unitNormalToPlane)
{
	float distance = math::DotProduct(unitNormalToPlane, point) - math::DotProduct(unitNormalToPlane, planeP);
	return distance;
}

int ClipTriangleAgainstPlane(const Plane<float> &plane, Triangle4d &inputTriangle, Triangle4d &outputTriangle1, Triangle4d &outputTriangle2)
{
	math::Vec4<float> unitNormalToPlane = math::UnitVector(plane.normal);

	// Two baskets to store points that are inside the plane and points that are outside
	math::Vec4<float>* insidePoints[3];
	math::Vec4<float>* outsidePoints[3];
	int insidePointCount = 0;
	int outsidePointCount = 0;

	// Work out the distance between the plane and each point on the triangle and put it in the relevant basket
	for (int i = 0; i < 3; i += 1)
	{
		float distance = ShortestDistanceFromPointToPlane(inputTriangle.p[i], plane.position, unitNormalToPlane);
		if (distance >= 0)
		{
			insidePoints[insidePointCount] = &inputTriangle.p[i];
			insidePointCount += 1;
		}
		else
		{
			outsidePoints[outsidePointCount] = &inputTriangle.p[i];
			outsidePointCount += 1;
		}
	}

	// All the points in the triangle are outside the plane
	if (insidePointCount == 0)
	{
		// inputTriangle is not valid.
		return 0;
	}

	// All the points on the triangle are inside the plane
	if (insidePointCount == 3)
	{
		// inputTriangle is valid.
		outputTriangle1 = inputTriangle;
		return 1;
	}

	// Two points lie outside the plane
	if (insidePointCount == 1 && outsidePointCount == 2)
	{
		// keep the one point inside the place in the output triangle
		outputTriangle1.p[0] = *insidePoints[0];

		// for the other two points, work out where the triangleintersects the plane and use those points in hte output
		outputTriangle1.p[1] = IntersectPlane(plane.position, unitNormalToPlane, *insidePoints[0], *outsidePoints[0]);
		outputTriangle1.p[2] = IntersectPlane(plane.position, unitNormalToPlane, *insidePoints[0], *outsidePoints[1]);
		return 1;
	}

	// One point lies outside the plane
	if (insidePointCount == 2 && outsidePointCount == 1)
	{
		// Create two new triangles from the two points inside the plane and the two points where the triangle intersects the plane
		outputTriangle1.p[0] = *insidePoints[0];
		outputTriangle1.p[1] = *insidePoints[1];
		outputTriangle1.p[2] = IntersectPlane(plane.position, unitNormalToPlane, *insidePoints[0], *outsidePoints[0]);

		outputTriangle2.p[0] = *insidePoints[1];
		outputTriangle2.p[1] = outputTriangle1.p[2];
		outputTriangle2.p[2] = IntersectPlane(plane.position, unitNormalToPlane, *insidePoints[1], *outsidePoints[0]);
		return 2;
	}

	return 0;
}
