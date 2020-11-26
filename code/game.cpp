#include <math.h>
#include "win32_platform.cpp"
#include "software_rendering.cpp"

#include "math.hpp"
#include "geometry.hpp"
#include "geometry.cpp"

#include "obj_file_reader.cpp"

math::Vec3<float> camera = {0};
Mesh mesh;
math::Matrix4x4<float> projectionMatrix;

float theta = 0.0f;

void GameInitialize(const GameMemory &gameMemory, const RenderBuffer &renderBuffer)
{
	// Using a clockwise winding convention
	/*mesh.triangles = {
		// SOUTH
		{ 0.0f, 0.0f, 0.0f,		0.0f, 1.0f, 0.0f,		1.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f,		1.0f, 1.0f, 0.0f,		1.0f, 0.0f, 0.0f },

		// EAST
		{ 1.0f, 0.0f, 0.0f,		1.0f, 1.0f, 0.0f,		1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f,		1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f },

		// NORTH
		{ 1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f },

		// WEST
		{ 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f, 0.0f },

		// TOP
		{ 0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f,		1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 0.0f },

		// BOTTOM
		{ 1.0f, 0.0f, 1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f, 0.0f }
	};*/

	ReadObjFileToVec3("teapot.obj", mesh.triangles);

	// Initialize the projection matrix
	projectionMatrix = MakeProjectionMatrix(90.0f, 1.0f, 0.1f, 1000.0f);
}

void GameUpdateAndRender(const GameMemory &gameMemory, const Input &input, const RenderBuffer &renderBuffer, float dt)
{
	const int RED = 0;
	const int GREEN = 255;
	const int BLUE = 0;
	const uint32_t BACKGROUND_COLOR = 0x000000;

	render::ClearScreen(renderBuffer, 0, 0, renderBuffer.width, renderBuffer.height, BACKGROUND_COLOR);

	theta += dt;
	// Initialize the rotation matrices
	math::Matrix4x4<float> rotationMatrixX = MakeXAxisRotationMatrix(theta);
	math::Matrix4x4<float> rotationMatrixZ = MakeZAxisRotationMatrix(theta);

	// Draw unit vectors along axes
	math::Vec2<int> origin = { 0, 0 };
	math::Vec2<int> xPos = { 100, 0 };
	math::Vec2<int> yPos = { 0, 100 };
	render::DrawLineInPixels(renderBuffer, 0xFFFFFF, origin, xPos);
	render::DrawLineInPixels(renderBuffer, 0xFFFFFF, origin, yPos);

	for (Triangle3d tri : mesh.triangles)
	{
		Triangle3d translate;
		Triangle3d rotatedZ;
		Triangle3d rotatedZX;
		Triangle2d projected;

		// Rotate each triangle
		math::MultiplyVectorWithMatrix(tri.p[0], rotatedZ.p[0], rotationMatrixZ);
		math::MultiplyVectorWithMatrix(tri.p[1], rotatedZ.p[1], rotationMatrixZ);
		math::MultiplyVectorWithMatrix(tri.p[2], rotatedZ.p[2], rotationMatrixZ);

		math::MultiplyVectorWithMatrix(rotatedZ.p[0], rotatedZX.p[0], rotationMatrixX);
		math::MultiplyVectorWithMatrix(rotatedZ.p[1], rotatedZX.p[1], rotationMatrixX);
		math::MultiplyVectorWithMatrix(rotatedZ.p[2], rotatedZX.p[2], rotationMatrixX);

		// Push back away from the camera which is implicitly located at z: 0. This ensures we're not trying to render trinagles behind the camera
		translate = rotatedZX;
		float zOffset = 150.0f;
		translate.p[0].z += zOffset;
		translate.p[1].z += zOffset;
		translate.p[2].z += zOffset;

		math::Vec3<float> normal;
		math::Vec3<float> line1;
		math::Vec3<float> line2;
		line1.x = translate.p[1].x - translate.p[0].x;
		line1.y = translate.p[1].y - translate.p[0].y;
		line1.z = translate.p[1].z - translate.p[0].z;

		line2.x = translate.p[2].x - translate.p[0].x;
		line2.y = translate.p[2].y - translate.p[0].y;
		line2.z = translate.p[2].z - translate.p[0].z;

		normal = math::UnitVector(math::CrossProduct(line1, line2));

		math::Vec3<float> fromCameraToTriangle = math::SubtractVectors(translate.p[0], camera);
		float dot = DotProduct(normal, fromCameraToTriangle);

		if (dot > 0.0f)
		{
			math::Vec3<float> lightDirection = { 0.0f, 0.0f, -1.0f };
			math::Vec3<float> normalizedLightDirection = UnitVector(lightDirection);
			float shade = math::DotProduct(normal, normalizedLightDirection);
			// Use bitwise operators to construct a single uint32_t value from three 0-255 RGB values.
			// There are 32 bits to fill up.
			// Each 0-255 value is a single byte, or 8 bits. So the 32 bits will be split into 4 segments (32 bits / 8 bits = 4).
			// |--1--|--2--|--3--|--4--|	bytes
			// 1     8     16    24    32	bits
			//
			// So the 0xRRGGBB value is made by constructing:
			// 1. the RR value and shifting it 16 places to the left
			//		|00|00|00|RR|	--> |00|RR|00|00|
			// 2. the GG value and shifting it 8 places to the left
			//		|00|00|00|GG|	-->	|00|00|GG|00|
			// 3. the BB value
			//							|00|00|00|BB|
			// Adding these together gives us the 0xRRGGBB value:
			//		|0x|00|00|00| + |00|RR|00|00| + |00|00|GG|00| + |00|00|00|BB| = |0x|RR|GG|BB|
			uint32_t triangleColor = (uint32_t)(0x000000 + (int(RED * shade) << 16) + (int(GREEN * shade) << 8) + int(BLUE * shade));
			
			// Project each triangle in 3D space onto the 2D space triangle to render
			math::ProjectVec3ToVec2(translate.p[0], projected.p[0], projectionMatrix);
			math::ProjectVec3ToVec2(translate.p[1], projected.p[1], projectionMatrix);
			math::ProjectVec3ToVec2(translate.p[2], projected.p[2], projectionMatrix);

			// Scale to view
			const float sf = 500.0f;
			Triangle2d triToRender = projected;
			triToRender.p[0].x *= sf;
			triToRender.p[0].y *= sf;
			triToRender.p[1].x *= sf;
			triToRender.p[1].y *= sf;
			triToRender.p[2].x *= sf;
			triToRender.p[2].y *= sf;

			const float translateX = 0.5f * (float)renderBuffer.width;
			const float translateY = 0.5f * (float)renderBuffer.height;
			triToRender.p[0].x += translateX; triToRender.p[0].y += translateY;
			triToRender.p[1].x += translateX; triToRender.p[1].y += translateY;
			triToRender.p[2].x += translateX; triToRender.p[2].y += translateY;

			math::Vec2<int> p0Int = { (int)triToRender.p[0].x, (int)triToRender.p[0].y };
			math::Vec2<int> p1Int = { (int)triToRender.p[1].x, (int)triToRender.p[1].y };
			math::Vec2<int> p2Int = { (int)triToRender.p[2].x, (int)triToRender.p[2].y };

			render::DrawTriangleInPixels(renderBuffer, triangleColor, p0Int, p1Int, p2Int);
		}
	}
}
