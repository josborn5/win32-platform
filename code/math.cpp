#include "math.hpp"
#include <math.h>

namespace gentle
{
	template<typename T>
	Vec4<T> AddVectors(const Vec4<T> &v1, const Vec4<T> &v2)
	{
		return Vec4<T>{v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	}
	template Vec4<int> AddVectors(const Vec4<int> &v1, const Vec4<int> &v2);
	template Vec4<float> AddVectors(const Vec4<float> &v1, const Vec4<float> &v2);
	template Vec4<double> AddVectors(const Vec4<double> &v1, const Vec4<double> &v2);

	template<typename T>
	Vec3<T> AddVectors(const Vec3<T> &v1, const Vec3<T> &v2)
	{
		return Vec3<T>{v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	}
	template Vec3<int> AddVectors(const Vec3<int> &v1, const Vec3<int> &v2);
	template Vec3<float> AddVectors(const Vec3<float> &v1, const Vec3<float> &v2);
	template Vec3<double> AddVectors(const Vec3<double> &v1, const Vec3<double> &v2);
}