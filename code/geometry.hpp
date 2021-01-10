#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "math.hpp"
#include <vector>

namespace gentle
{
	template<typename T>
	struct Triangle2d
	{
		Vec2<T> p[3];
	};

	template<typename T>
	struct Triangle4d
	{
		Vec4<T> p[3];
		uint32_t color;
	};

	template<typename T>
	struct Plane
	{
		Vec3<T> position;
		Vec3<T> normal;
	};

	template<typename T>
	struct Mesh
	{
		std::vector<Triangle4d<T>> triangles;
	};

	template<typename T>
	struct Camera
	{
		Vec4<T> position;
		Vec4<T> direction;
		Vec4<T> up;
	};

	template<typename T>
	Matrix4x4<T> MakeIdentityMatrix()
	{
		Matrix4x4<T> matrix;
		matrix.m[0][0] = 1;
		matrix.m[1][1] = 1;
		matrix.m[2][2] = 1;
		matrix.m[3][3] = 1;
		return matrix;
	}

	template<typename T>
	Matrix4x4<T> MakeTranslationMatrix(T dispX, T dispY, T dispZ)
	{
		Matrix4x4<T> matrix = MakeIdentityMatrix<T>();
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
	template<typename T>
	Matrix4x4<T> PointAt(const Vec4<T> &position, const Vec4<T> &target, const Vec4<T> &up)
	{
		// Vector from the position to the target is the new forward direction
		Vec4<T> forwardUnit = SubtractVectors(target, position);
		forwardUnit = UnitVector(forwardUnit);

		// Calculate the new up direction of the new forward direction
		T newUpScalar = DotProduct(up, forwardUnit);
		Vec4<T> newUpTemp = MultiplyVectorByScalar(forwardUnit, newUpScalar);
		Vec4<T> upUnit = SubtractVectors(up, newUpTemp);
		upUnit = UnitVector(upUnit);

		// Calculate the new right direction for the new up & forward directions
		Vec4<T> rightUnit = CrossProduct(upUnit, forwardUnit);

		// Construct the new transformation matrix
		Matrix4x4<T> pointAt;
		pointAt.m[0][0] = rightUnit.x;		pointAt.m[0][1] = rightUnit.y;		pointAt.m[0][2] = rightUnit.z;		pointAt.m[0][3] = 0;
		pointAt.m[1][0] = upUnit.x;			pointAt.m[1][1] = upUnit.y;			pointAt.m[1][2] = upUnit.z;			pointAt.m[1][3] = 0;
		pointAt.m[2][0] = forwardUnit.x;	pointAt.m[2][1] = forwardUnit.y;	pointAt.m[2][2] = forwardUnit.z;	pointAt.m[2][3] = 0;
		pointAt.m[3][0] = position.x;		pointAt.m[3][1] = position.y;		pointAt.m[3][2] = position.z;		pointAt.m[3][3] = 1;
		return pointAt;
	}

	/**
	* Structure of the LookAt Matrix:
	* |  Ax  |  Bx  |  Cx  | 0 |
	* |  Ay  |  By  |  Cy  | 0 |
	* |  Az  |  Bz  |  Cz  | 0 |
	* | -T.A | -T.B | -T.C | 1 |
	*/
	template<typename T>
	Matrix4x4<T> LookAt(Matrix4x4<T> const &pointAt)
	{
		T tDotA = (pointAt.m[3][0] * pointAt.m[0][0]) + (pointAt.m[3][1] * pointAt.m[0][1]) + (pointAt.m[3][2] * pointAt.m[0][2]);
		T tDotB = (pointAt.m[3][0] * pointAt.m[1][0]) + (pointAt.m[3][1] * pointAt.m[1][1]) + (pointAt.m[3][2] * pointAt.m[1][2]);
		T tDotC = (pointAt.m[3][0] * pointAt.m[2][0]) + (pointAt.m[3][1] * pointAt.m[2][1]) + (pointAt.m[3][2] * pointAt.m[2][2]);

		Matrix4x4<T> lookAt;
		lookAt.m[0][0] = pointAt.m[0][0];	lookAt.m[0][1] = pointAt.m[1][0];	lookAt.m[0][2] = pointAt.m[2][0];	lookAt.m[0][3] = 0;
		lookAt.m[1][0] = pointAt.m[0][1];	lookAt.m[1][1] = pointAt.m[1][1];	lookAt.m[1][2] = pointAt.m[2][1];	lookAt.m[1][3] = 0;
		lookAt.m[2][0] = pointAt.m[0][2];	lookAt.m[2][1] = pointAt.m[1][2];	lookAt.m[2][2] = pointAt.m[2][2];	lookAt.m[2][3] = 0;
		lookAt.m[3][0] = -tDotA;			lookAt.m[3][1] = -tDotB;			lookAt.m[3][2] = -tDotC;			lookAt.m[3][3] = 1;
		return lookAt;
	}

	template<typename T>
	Vec4<T> IntersectPlane(const Plane<T> &plane, const Vec4<T> &lineStart, const Vec4<T> lineEnd)
	{
		Vec3<T> normalizedPlaneN = UnitVector(plane.normal);
		T planeD = DotProduct(normalizedPlaneN, plane.position);
		T ad = DotProduct(normalizedPlaneN, lineStart);
		T bd = DotProduct(normalizedPlaneN, lineEnd);
		T t = (planeD - ad) / (bd - ad);
		Vec4<T> lineStartToEnd = SubtractVectors(lineEnd, lineStart);
		Vec4<T> lineToIntersect = MultiplyVectorByScalar(lineStartToEnd, t);
		return AddVectors(lineStart, lineToIntersect);
	}

	template<typename T>
	T ShortestDistanceFromPointToPlane(const Vec4<T> &point, const Vec3<T> &planeP, const Vec3<T> &unitNormalToPlane)
	{
		T distance = DotProduct(unitNormalToPlane, point) - DotProduct(unitNormalToPlane, planeP);
		return distance;
	}

	template<typename T>
	int ClipTriangleAgainstPlane(const Plane<T> &plane, Triangle4d<T> &inputTriangle, Triangle4d<T> &outputTriangle1, Triangle4d<T> &outputTriangle2)
	{
		Vec3<T> unitNormalToPlane = UnitVector(plane.normal);

		// Two baskets to store points that are inside the plane and points that are outside
		Vec4<T>* insidePoints[3];
		Vec4<T>* outsidePoints[3];
		int insidePointCount = 0;
		int outsidePointCount = 0;

		// Work out the distance between the plane and each point on the triangle and put it in the relevant basket
		for (int i = 0; i < 3; i += 1)
		{
			T distance = ShortestDistanceFromPointToPlane(inputTriangle.p[i], plane.position, unitNormalToPlane);
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
			outputTriangle1.p[1] = IntersectPlane(plane, *insidePoints[0], *outsidePoints[0]);
			outputTriangle1.p[2] = IntersectPlane(plane, *insidePoints[0], *outsidePoints[1]);
			outputTriangle1.color = inputTriangle.color;
			return 1;
		}

		// One point lies outside the plane
		if (insidePointCount == 2 && outsidePointCount == 1)
		{
			// Create two new triangles from the two points inside the plane and the two points where the triangle intersects the plane
			outputTriangle1.p[0] = *insidePoints[0];
			outputTriangle1.p[1] = *insidePoints[1];
			outputTriangle1.p[2] = IntersectPlane(plane, *insidePoints[0], *outsidePoints[0]);
			outputTriangle1.color = inputTriangle.color;

			outputTriangle2.p[0] = *insidePoints[1];
			outputTriangle2.p[1] = outputTriangle1.p[2];
			outputTriangle2.p[2] = IntersectPlane(plane, *insidePoints[1], *outsidePoints[0]);
			outputTriangle2.color = inputTriangle.color;
			return 2;
		}

		return 0;
	}

	Matrix4x4<float> MakeProjectionMatrix(float fieldOfVewDeg, float aspectRatio, float nearPlane, float farPlane);

	void SetZAxisRotationMatrix(float theta, Matrix4x4<float> &matrix);

	Matrix4x4<float> MakeZAxisRotationMatrix(float theta);

	void SetYAxisRotationMatrix(float theta, Matrix4x4<float> &matrix);

	Matrix4x4<float> MakeYAxisRotationMatrix(float theta);

	void SetXAxisRotationMatrix(float theta, Matrix4x4<float> &matrix);

	Matrix4x4<float> MakeXAxisRotationMatrix(float theta);
}

#endif