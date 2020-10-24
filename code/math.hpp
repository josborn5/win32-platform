#ifndef MATH_H
#define MATH_H

namespace math
{
	template<typename T>
	struct Vec2
	{
		T x;
		T y;
	};

	template<typename T>
	struct Vec3
	{
		T x;
		T y;
		T z;
	};

	template<typename T>
	T dot_product(const Vec3<T> &v1, const Vec3<T> &v2)
	{
		return
			(v1.x * v2.x) +
			(v1.y * v2.y) +
			(v1.z * v2.z);
	}

	template<typename T>
	Vec3<T> cross_product(const Vec3<T> &v1, const Vec3<T> &v2)
	{
		return Vec3<T>(
			(v1.y * v2.z) - (v1.z * v2.y),
			(v1.z * v2.x) - (v1.x * v2.z),
			(v1.x * v2.y) - (v1.y * v2.x)
		);
	}

	template<typename T>
	struct Matrix4x4
	{
		T m[4][4] = {0};
	};

	template<typename T>
	void ProjectVec3ToVec2(const Vec3<T> &in, Vec2<T> &out, const Matrix4x4<T> &matrix)
	{
		out.x = (in.x * matrix.m[0][0]) + (in.y * matrix.m[1][0]) + (in.z * matrix.m[2][0]) + matrix.m[3][0];
		out.y = (in.x * matrix.m[0][1]) + (in.y * matrix.m[1][1]) + (in.z * matrix.m[2][1]) + matrix.m[3][1];
		// out.z = (in.x * matrix.m[0][2]) + (in.y * matrix.m[1][2]) + (in.z * matrix.m[2][2]) + matrix.m[3][2];
		float w = (in.x * matrix.m[0][3]) + (in.y * matrix.m[1][3]) + (in.z * matrix.m[2][3]) + matrix.m[3][3];

		if (w != 0.0f)
		{
			out.x /= w;
			out.y /= w;
			// out.z /= w;
		}
	}

	template<typename T>
	void MultiplyVectorWithMatrix(const Vec3<T> &in, Vec3<T> &out, const Matrix4x4<T> &matrix)
	{
		out.x = (in.x * matrix.m[0][0]) + (in.y * matrix.m[1][0]) + (in.z * matrix.m[2][0]) + matrix.m[3][0];
		out.y = (in.x * matrix.m[0][1]) + (in.y * matrix.m[1][1]) + (in.z * matrix.m[2][1]) + matrix.m[3][1];
		out.z = (in.x * matrix.m[0][2]) + (in.y * matrix.m[1][2]) + (in.z * matrix.m[2][2]) + matrix.m[3][2];
		float w = (in.x * matrix.m[0][3]) + (in.y * matrix.m[1][3]) + (in.z * matrix.m[2][3]) + matrix.m[3][3];

		if (w != 0.0f)
		{
			out.x /= w;
			out.y /= w;
			out.z /= w;
		}
	}
}

#endif