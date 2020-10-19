#ifndef MATH_H
#define MATH_H

namespace math
{
	template<typename T>
	struct Vec2
	{
		T x;
		T y;

		Vec2(T valX, T valY)
		{
			x = valX;
			y = valY;
		}
	};

	template<typename T>
	struct Vec3
	{
		T x;
		T y;
		T z;

		Vec3(T valX, T valY, T valZ)
		{
			x = valX;
			y = valY;
			z = valZ;
		}
	};
}

#endif