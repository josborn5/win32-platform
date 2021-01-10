#ifndef FILE_H
#define FILE_H

#include "geometry.hpp"

namespace gentle
{
	template<typename T>
	bool ReadObjFileToVec4(std::string const &filename, std::vector<Triangle4d<T>> &triangles);
}

#endif