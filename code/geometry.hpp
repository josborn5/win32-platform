#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "math.hpp"
#include <vector>

struct Triangle2d
{
	math::Vec2<float> p[3];
};

struct Triangle4d
{
	math::Vec4<float> p[3];
	uint32_t color;
};

template<typename T>
struct Plane
{
	math::Vec4<T> position;
	math::Vec4<T> normal;
};

struct Mesh
{
	std::vector<Triangle4d> triangles;
};

#endif