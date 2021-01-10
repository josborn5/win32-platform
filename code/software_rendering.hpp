#ifndef SOFTWARE_RENDERING_H
#define SOFTWARE_RENDERING_H

#include "platform.hpp"
#include "math.hpp"
#include "geometry.hpp"

namespace gentle
{
	/**
	 *	|---|---|---|
	 *	| 0 | 1 | 2 |	pixel ordinals
	 *	|---|---|---|
	 *	0   1   2   3	position ordinals
	 *
	 * x & y parameters are the pixel and NOT the position ordinals
	 */
	static void PlotPixel(const RenderBuffer &renderBuffer, uint32_t color, int x, int y);

	/**
	 *	|---|---|---|
	 *	| 0 | 1 | 2 |	pixel ordinals
	 *	|---|---|---|
	 *	0   1   2   3	position ordinals
	 *
	 * p0 & p1 are pixel and NOT position ordinals
	 */
	// Implemented with Bresenham's algorithm
	static void DrawLineInPixels(const RenderBuffer &renderBuffer, uint32_t color, const Vec2<int> &p0, const Vec2<int> &p1);

	void FillTriangleInPixels(const RenderBuffer &renderBuffer, uint32_t color, const Vec3<int> &p0, const Vec3<int> &p1, const Vec3<int> &p2, float z);


	void DrawTriangleInPixels(const RenderBuffer &renderBuffer, uint32_t color, const Vec2<int> &p0, const Vec2<int> &p1, const Vec2<int> &p2);

	void ClearScreen(const RenderBuffer &renderBuffer, uint32_t color);

	template<typename T>
	void TransformAndRenderMesh(const RenderBuffer &renderBuffer, const Mesh<T> &mesh, const Camera<T> &camera, const Matrix4x4<T> transformMatrix)
	{
		const int RED = 0;
		const int GREEN = 255;
		const int BLUE = 0;

		// Camera matrix
		Vec4<T> target = AddVectors(camera.position, camera.direction);
		Matrix4x4<float> cameraMatrix = PointAt(camera.position, target, camera.up);

		// View matrix
		Matrix4x4<float> viewMatrix = LookAt(cameraMatrix);

		std::vector<Triangle4d<float>> trianglesToDraw;

		for (Triangle4d<float> tri : mesh.triangles)
		{
			Triangle4d<float> transformed;
			Triangle4d<float> viewed;
			Triangle4d<float> projected; // TODO: switch this to Triangle4d so the depth information is kept and can be used in a depth buffer to prevent double rendering of triangles behind each other

			// Transform the triangle in the mesh
			MultiplyVectorWithMatrix(tri.p[0], transformed.p[0], transformMatrix);
			MultiplyVectorWithMatrix(tri.p[1], transformed.p[1], transformMatrix);
			MultiplyVectorWithMatrix(tri.p[2], transformed.p[2], transformMatrix);

			// Work out the normal of the triangle
			Vec4<float> line1 = SubtractVectors(transformed.p[1], transformed.p[0]);
			Vec4<float> line2 = SubtractVectors(transformed.p[2], transformed.p[0]);
			Vec4<float> normal = UnitVector(CrossProduct(line1, line2));

			Vec4<float> fromCameraToTriangle = SubtractVectors(transformed.p[0], camera.position);
			float dot = DotProduct(normal, fromCameraToTriangle);

			if (dot >= 0.0f)
			{
				Vec4<float> lightDirection = { 0.0f, 0.0f, 1.0f };
				Vec4<float> normalizedLightDirection = UnitVector(lightDirection);
				float shade = DotProduct(normal, normalizedLightDirection);
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
				MultiplyVectorWithMatrix(transformed.p[0], viewed.p[0], viewMatrix);
				MultiplyVectorWithMatrix(transformed.p[1], viewed.p[1], viewMatrix);
				MultiplyVectorWithMatrix(transformed.p[2], viewed.p[2], viewMatrix);

				// Clip the triangles before they get projected. Define a plane just in fron of the camera to clip against
				Triangle4d<float> clipped[2];
				Plane<float> inFrontOfScreen = { 0.0f, 0.0f, 0.1f,	 0.0f, 0.0f, 1.0f };
				int clippedTriangleCount = ClipTriangleAgainstPlane(inFrontOfScreen, viewed, clipped[0], clipped[1]);

				for (int i = 0; i < clippedTriangleCount; i += 1)
				{
					// Project each triangle in 3D space onto the 2D space triangle to render
					Project3DPointTo2D(clipped[i].p[0], projected.p[0], projectionMatrix);
					Project3DPointTo2D(clipped[i].p[1], projected.p[1], projectionMatrix);
					Project3DPointTo2D(clipped[i].p[2], projected.p[2], projectionMatrix);

					// Scale to view
					const float sf = 500.0f;
					Triangle4d<float> triToRender = projected;
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

		for (Triangle4d<float> triToRender : trianglesToDraw)
		{
			Triangle4d<float> clipped[2];
			std::list<Triangle4d<float>> triangleQueue;
			triangleQueue.push_back(triToRender);
			int newTriangles = 1;

			// Clip against each screen edge
			for (int edge = 0; edge < 4; edge += 1)
			{
				int trianglesToAdd = 0;
				while (newTriangles > 0)
				{
					Triangle4d<float> test = triangleQueue.front();
					triangleQueue.pop_front();
					newTriangles -= 1;

					Plane<float> bottomOfScreen = { 0.0f, 0.0f, 0.0f,								0.0f, 1.0f, 0.0f };
					Plane<float> topOfScreen = { 0.0f, (float)(renderBuffer.height - 1), 0.0f,		0.0f, -1.0f, 0.0f };
					Plane<float> leftOfScreen = { 0.0f, 0.0f, 0.0f,									1.0f, 0.0f, 0.0f };
					Plane<float> rightOfScreen = { (float)(renderBuffer.width - 1), 0.0f, 0.0f,		-1.0f, 0.0f, 0.0f };
					switch (edge)
					{
						case 0:
						{
							trianglesToAdd = ClipTriangleAgainstPlane(bottomOfScreen, test, clipped[0], clipped[1]);
							break;
						}
						case 1:
						{
							trianglesToAdd = ClipTriangleAgainstPlane(topOfScreen, test, clipped[0], clipped[1]);
							break;
						}
						case 2:
						{
							trianglesToAdd = ClipTriangleAgainstPlane(leftOfScreen, test, clipped[0], clipped[1]);
							break;
						}
						case 3:
						{
							trianglesToAdd = ClipTriangleAgainstPlane(rightOfScreen, test, clipped[0], clipped[1]);
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

			for (Triangle4d<float> draw : triangleQueue)
			{
				// Vec2<int> p0Int = { (int)draw.p[0].x, (int)draw.p[0].y };
				// Vec2<int> p1Int = { (int)draw.p[1].x, (int)draw.p[1].y };
				// Vec2<int> p2Int = { (int)draw.p[2].x, (int)draw.p[2].y };
				// DrawTriangleInPixels(renderBuffer, draw.color, p0Int, p1Int, p2Int);

				Vec3<int> p0Int = { (int)draw.p[0].x, (int)draw.p[0].y };
				Vec3<int> p1Int = { (int)draw.p[1].x, (int)draw.p[1].y };
				Vec3<int> p2Int = { (int)draw.p[2].x, (int)draw.p[2].y };

				// Super rough, take the depth as the average z value
				// For whatever reason, the z values are inverted for the teapot. i.e. closer triangles have a lower Z value.
				// As an ultra-hack I'm doing 10 minus the z-value to invert them.
				float z = 10.0f - ((draw.p[0].z + draw.p[1].z + draw.p[2].z) / 3.0f);
				FillTriangleInPixels(renderBuffer, draw.color, p0Int, p1Int, p2Int, z);
			}
		}
	}
}

#endif