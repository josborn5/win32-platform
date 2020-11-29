#include <math.h>
#include <list>
#include "win32_platform.cpp"
#include "software_rendering.cpp"

#include "math.hpp"
#include "geometry.hpp"
#include "geometry.cpp"

#include "obj_file_reader.cpp"

math::Vec3<float> cameraPosition = {0};
math::Vec3<float> cameraDirection = {0};
Mesh mesh;
math::Matrix4x4<float> projectionMatrix;

float theta = 0.0f;
float cameraYaw = 0.0f;

void GameInitialize(const GameMemory &gameMemory, const RenderBuffer &renderBuffer)
{
	ReadObjFileToVec3("teapot.obj", mesh.triangles);
	
	// Using a clockwise winding convention
	/*mesh.triangles = {
		// SOUTH
		{ 0.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f },

		// EAST
		{ 1.0f, 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f, 1.0f },

		// NORTH
		{ 1.0f, 0.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f },

		// WEST
		{ 0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 0.0f, 0.0f, 1.0f },

		// TOP
		{ 0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f },

		// BOTTOM
		{ 1.0f, 0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f, 1.0f }
	};*/

	

	// Initialize the projection matrix
	projectionMatrix = MakeProjectionMatrix(90.0f, 1.0f, 0.1f, 1000.0f);

	// Initialize the camera
	cameraDirection = { 0.0f, 0.0f, 1.0f };
}

void GameUpdateAndRender(const GameMemory &gameMemory, const Input &input, const RenderBuffer &renderBuffer, float dt)
{
	const int RED = 0;
	const int GREEN = 255;
	const int BLUE = 0;
	const uint32_t BACKGROUND_COLOR = 0x000000;

	float positionIncrement = 1.0f;
	float yawIncrement = 0.05f;

	// First process any change in yaw and update the camera direction
	if (input.buttons[KEY_D].isDown)
	{
		cameraYaw -= yawIncrement;
	}
	else if (input.buttons[KEY_A].isDown)
	{
		cameraYaw += yawIncrement;
	}

	// Apply the camera yaw to the cameraDirection vector
	math::Vec3<float> up = { 0.0f, 1.0f, 0.0f };
	math::Vec3<float> target = { 0.0f, 0.0f, 1.0f };
	math::Matrix4x4<float> cameraYawMatrix = MakeYAxisRotationMatrix(cameraYaw);
	math::MultiplyVectorWithMatrix(target, cameraDirection, cameraYawMatrix);

	// Next process any forwards or backwards movement
	math::Vec3<float> cameraPositionForwardBack = MultiplyVectorByScalar(cameraDirection, positionIncrement);
	if (input.buttons[KEY_S].isDown)
	{
		cameraPosition = SubtractVectors(cameraPosition, cameraPositionForwardBack);
	}
	else if (input.buttons[KEY_W].isDown)
	{
		cameraPosition = AddVectors(cameraPosition, cameraPositionForwardBack);
	}

	// Strafing - use the cross product between the camera direction and up to get a normal vector to the direction being faced
	math::Vec3<float> cameraPositionStrafe = CrossProduct(up, cameraDirection);
	if (input.buttons[KEY_LEFT].isDown)
	{
		cameraPosition = SubtractVectors(cameraPosition, cameraPositionStrafe);
	}
	else if (input.buttons[KEY_RIGHT].isDown)
	{
		cameraPosition = AddVectors(cameraPosition, cameraPositionStrafe);
	}

	// Simply move the camera position vertically with up/down keypress
	if (input.buttons[KEY_DOWN].isDown)
	{
		cameraPosition.y -= positionIncrement;
	}
	else if (input.buttons[KEY_UP].isDown)
	{
		cameraPosition.y += positionIncrement;
	}

	render::ClearScreen(renderBuffer, 0, 0, renderBuffer.width, renderBuffer.height, BACKGROUND_COLOR);

	theta += dt;
	// Initialize the rotation matrices
	math::Matrix4x4<float> rotationMatrixX = MakeXAxisRotationMatrix(theta);
	math::Matrix4x4<float> rotationMatrixY = MakeYAxisRotationMatrix(theta);
	math::Matrix4x4<float> rotationMatrixZ = MakeZAxisRotationMatrix(theta);

	// Initialize the translation matrix
	// Push back away from the camera which is implicitly located at z: 0. This ensures we're not trying to render trinagles behind the camera
	math::Matrix4x4<float> translationMatrix = MakeTranslationMatrix(0.0f, 0.0f, 150.0f);

	// Combine all the rotation and translation matrices into a single world transfomration matrix
	math::Matrix4x4<float> worldMatrix;
	// worldMatrix = MultiplyMatrixWithMatrix(rotationMatrixZ, rotationMatrixX);
	worldMatrix = MakeIdentityMatrix();
	worldMatrix = MultiplyMatrixWithMatrix(worldMatrix, translationMatrix);

	// Camera matrix
	target = AddVectors(cameraPosition, cameraDirection);
	math::Matrix4x4<float> cameraMatrix = PointAt(cameraPosition, target, up);

	// View matrix
	math::Matrix4x4<float> viewMatrix = LookAt(cameraMatrix);

	// Draw unit vectors along axes
	math::Vec2<int> origin = { 0, 0 };
	math::Vec2<int> xPos = { 100, 0 };
	math::Vec2<int> yPos = { 0, 100 };
	render::DrawLineInPixels(renderBuffer, 0xFFFFFF, origin, xPos);
	render::DrawLineInPixels(renderBuffer, 0xFFFFFF, origin, yPos);

	std::vector<Triangle3d> trianglesToDraw;

	for (Triangle3d tri : mesh.triangles)
	{
		Triangle3d transformed;
		Triangle3d viewed;
		Triangle3d projected; // TODO: switch this to Triangle3d so the depth information is kept and can be used in a depth buffer to prevent double rendering of triangles behind each other

		// Transform each triangle
		math::MultiplyVectorWithMatrix(tri.p[0], transformed.p[0], worldMatrix);
		math::MultiplyVectorWithMatrix(tri.p[1], transformed.p[1], worldMatrix);
		math::MultiplyVectorWithMatrix(tri.p[2], transformed.p[2], worldMatrix);

		math::Vec3<float> line1 = SubtractVectors(transformed.p[1], transformed.p[0]);
		math::Vec3<float> line2 = SubtractVectors(transformed.p[2], transformed.p[0]);
		math::Vec3<float> normal = math::UnitVector(math::CrossProduct(line1, line2));

		math::Vec3<float> fromCameraToTriangle = math::SubtractVectors(transformed.p[0], cameraPosition);
		float dot = DotProduct(normal, fromCameraToTriangle);

		if (dot >= 0.0f)
		{
			math::Vec3<float> lightDirection = { 0.0f, 0.0f, 1.0f };
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

			// Convert the triangle position from world space to view space
			math::MultiplyVectorWithMatrix(transformed.p[0], viewed.p[0], viewMatrix);
			math::MultiplyVectorWithMatrix(transformed.p[1], viewed.p[1], viewMatrix);
			math::MultiplyVectorWithMatrix(transformed.p[2], viewed.p[2], viewMatrix);

			// Clip the triangles before they get projected. Define a plane just in fron of the camera to clip against
			Triangle3d clipped[2];
			int clippedTriangleCount = ClipTriangleAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, viewed, clipped[0], clipped[1]);

			for (int i = 0; i < clippedTriangleCount; i += 1)
			{
				// Project each triangle in 3D space onto the 2D space triangle to render
				math::Project3DPointTo2D(clipped[i].p[0], projected.p[0], projectionMatrix);
				math::Project3DPointTo2D(clipped[i].p[1], projected.p[1], projectionMatrix);
				math::Project3DPointTo2D(clipped[i].p[2], projected.p[2], projectionMatrix);

				// Scale to view
				const float sf = 500.0f;
				Triangle3d triToRender = projected;
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

				triToRender.color = triangleColor;

				trianglesToDraw.push_back(triToRender);
			}
		}
	}

	for (Triangle3d triToRender : trianglesToDraw)
	{
		Triangle3d clipped[2];
		std::list<Triangle3d> triangleQueue;
		triangleQueue.push_back(triToRender);
		int newTriangles = 1;

		// Clip against each screen edge
		for (int edge = 0; edge < 4; edge += 1)
		{
			int trianglesToAdd = 0;
			while (newTriangles > 0)
			{
				Triangle3d test = triangleQueue.front();
				triangleQueue.pop_front();
				newTriangles -= 1;

				switch (edge)
				{
					case 0:
					{
						trianglesToAdd = ClipTriangleAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]);
						break;
					}
					case 1:
					{
						trianglesToAdd = ClipTriangleAgainstPlane({ 0.0f, (float)(renderBuffer.height - 1), 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]);
						break;
					}
					case 2:
					{
						trianglesToAdd = ClipTriangleAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]);
						break;
					}
					case 3:
					{
						trianglesToAdd = ClipTriangleAgainstPlane({ (float)(renderBuffer.width - 1), 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]);
						break;
					}
				}

				for (int i = 0; i < trianglesToAdd; i += 1)
				{
					triangleQueue.push_back(clipped[i]);
				}
			}

			newTriangles = (int)triangleQueue.size();
		}

		for (Triangle3d draw : triangleQueue)
		{
			math::Vec2<int> p0Int = { (int)triToRender.p[0].x, (int)triToRender.p[0].y };
			math::Vec2<int> p1Int = { (int)triToRender.p[1].x, (int)triToRender.p[1].y };
			math::Vec2<int> p2Int = { (int)triToRender.p[2].x, (int)triToRender.p[2].y };

			render::DrawTriangleInPixels(renderBuffer, draw.color, p0Int, p1Int, p2Int);
		}
	}
}
