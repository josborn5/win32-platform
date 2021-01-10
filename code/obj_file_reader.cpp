
#include <fstream>
#include <strstream>
#include <string>
#include <iostream>
#include <vector>

template<typename T>
static bool ReadObjFileToVec4(std::string const &filename, std::vector<Triangle4d<T>> &triangles)
{
	std::ifstream objFile;
	objFile.open(filename);
	if (!objFile.is_open())
	{
		return false;
	}

	std::vector<gentle::Vec4<T>> vertices;

	while (!objFile.eof())
	{
		char junk;
		std::string line;
		std::getline(objFile, line);

		std::strstream stringStream;
		stringStream << line;

		if (line[0] == 'v')
		{
			gentle::Vec4<T> vertex;
			// expect line to have syntax 'v x y z' where x, y & z are the ordinals of the point position
			stringStream >> junk >> vertex.x >> vertex.y >> vertex.z;
			vertex.w = 1.0f;
			vertices.push_back(vertex);
		}

		if (line[0] == 'f')
		{
			int points[3];
			stringStream >> junk >> points[0] >> points[1] >> points[2];
			// expect line to have syntax 'f 1 2 3' where 1, 2 & 3 are the 1-indexed positions of the points in the file
			Triangle4d<T> newTriangle = { vertices[points[0] - 1], vertices[points[1] - 1], vertices[points[2] - 1] };
			triangles.push_back(newTriangle);
		}
	}

	objFile.close();

	return true;
}