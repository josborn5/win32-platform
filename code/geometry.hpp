#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "math.hpp"
#include <vector>

struct Triangle2d
{
	math::Vec2<float> p[3];
};

struct Triangle3d
{
	math::Vec3<float> p[3];
};

struct Mesh
{
	std::vector<Triangle3d> triangles;
};

#endif