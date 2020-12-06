#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "math.hpp"
#include <vector>

template<typename T>
struct Triangle2d
{
	math::Vec2<T> p[3];
};

template<typename T>
struct Triangle4d
{
	math::Vec4<T> p[3];
	uint32_t color;
};

template<typename T>
struct Plane
{
	math::Vec3<T> position;
	math::Vec3<T> normal;
};

template<typename T>
struct Mesh
{
	std::vector<Triangle4d<T>> triangles;
};

#endif