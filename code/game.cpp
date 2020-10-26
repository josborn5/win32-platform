#include <math.h>
#include "win32_platform.cpp"
#include "software_rendering.cpp"

#include "math.hpp"
#include "geometry.hpp"

math::Vec3<float> camera = {0};
Mesh mesh;
math::Matrix4x4<float> projectionMatrix;

float theta = 0.0f;

void GameInitialize(const GameMemory &gameMemory, const RenderBuffer &renderBuffer)
{
	// Using a clockwise winding convention
	mesh.triangles = {
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
	};

	// Initialize the projection matrix
	float nearPlane = 0.1f;
	float farPlane = 1000.0f;
	float fFieldOfViewDeg = 90.0f;
	// float fieldOfViewRadians = (90.0f * 3.14159f) / 180.0f;
	float aspectRatio = 1.0f;
	// float aspectRatio = (float)renderBuffer.width / (float)renderBuffer.height;
	float inverseTangent = 1.0f / std::tanf(fFieldOfViewDeg * 0.5f * 3.14159f / 180.0f);
	// float inverseTangent = 1.0f / std::tanf(0.5f * fieldOfViewRadians);

	projectionMatrix.m[0][0] = aspectRatio * inverseTangent;
	projectionMatrix.m[1][1] = inverseTangent;
	projectionMatrix.m[2][2] = farPlane / (farPlane - nearPlane);
	projectionMatrix.m[3][2] = (-farPlane * nearPlane) / (farPlane - nearPlane);
	projectionMatrix.m[2][3] = 1.0f;
	projectionMatrix.m[3][3] = 0.0f;
}

void GameUpdateAndRender(const GameMemory &gameMemory, const Input &input, const RenderBuffer &renderBuffer, float dt)
{
	const uint32_t TEXT_COLOR = 0x00FF00;
	const uint32_t BACKGROUND_COLOR = 0x000000;

	render::ClearScreen(renderBuffer, 0, 0, renderBuffer.width, renderBuffer.height, BACKGROUND_COLOR);

	theta += dt;
	math::Matrix4x4<float> rotationMatrixX;
	math::Matrix4x4<float> rotationMatrixZ;

	// Initialize the rotation matrices
	rotationMatrixZ.m[0][0] = std::cosf(theta);
	rotationMatrixZ.m[0][1] = -std::sinf(theta);
	rotationMatrixZ.m[1][0] = std::sinf(theta);
	rotationMatrixZ.m[1][1] = std::cosf(theta);
	rotationMatrixZ.m[2][2] = 1.0f;
	rotationMatrixZ.m[3][3] = 1.0f;

	rotationMatrixX.m[0][0] = 1.0f;
	rotationMatrixX.m[1][1] = std::cosf(theta);
	rotationMatrixX.m[1][2] = -std::sinf(theta);
	rotationMatrixX.m[2][1] = std::sinf(theta);
	rotationMatrixX.m[2][2] = std::cosf(theta);
	rotationMatrixX.m[3][3] = 1.0f;

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
		float zOffset = 3.0f;
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

		if (dot < 0.0f)
		{
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

			render::DrawTriangleInPixels(renderBuffer, TEXT_COLOR, p0Int, p1Int, p2Int);
		}
	}
}
