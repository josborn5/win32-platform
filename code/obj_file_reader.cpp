
#include <fstream>
#include <strstream>
#include <string>
#include <iostream>
#include <vector>

static bool ReadObjFileToVec3(std::string const &filename, std::vector<Triangle3d> &triangles)
{
	std::ifstream objFile;
	objFile.open(filename);
	if (!objFile.is_open())
	{
		return false;
	}

	std::vector<math::Vec3<float>> vertices;

	while (!objFile.eof())
	{
		char junk;
		std::string line;
		std::getline(objFile, line);

		std::strstream stringStream;
		stringStream << line;

		if (line[0] == 'v')
		{
			math::Vec3<float> vertex;
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
			Triangle3d newTriangle = { vertices[points[0] - 1], vertices[points[1] - 1], vertices[points[2] - 1] };
			triangles.push_back(newTriangle);
		}
	}

	objFile.close();

	return true;
}