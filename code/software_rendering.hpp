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
	void PlotPixel(const RenderBuffer &renderBuffer, uint32_t color, int x, int y);

	/**
	 *	|---|---|---|
	 *	| 0 | 1 | 2 |	pixel ordinals
	 *	|---|---|---|
	 *	0   1   2   3	position ordinals
	 *
	 * p0 & p1 are pixel and NOT position ordinals
	 */
	// Implemented with Bresenham's algorithm
	void DrawLineInPixels(const RenderBuffer &renderBuffer, uint32_t color, const Vec2<int> &p0, const Vec2<int> &p1);

	// Rects
	void DrawRect(const RenderBuffer &renderBuffer, uint32_t color, const Rect<float> &rect);

	// Draw a sprite of a size determined by the given sprite string and blockHalfSize value
	void DrawSprite(const RenderBuffer &renderBuffer, char *sprite, const Vec2<float> &p, float blockHalfSize, uint32_t color);

	// Draw a sprite of a fixed size determined by the given footPrint value
	void DrawSprite(
		const RenderBuffer &renderBuffer,
		char *sprite,
		const Rect<float> &footPrint,
		int xRes,
		int yRes,
		uint32_t color
	);

	// Triangles
	void FillTriangleInPixels(const RenderBuffer &renderBuffer, uint32_t color, const Vec3<int> &p0, const Vec3<int> &p1, const Vec3<int> &p2, float z);


	void DrawTriangleInPixels(const RenderBuffer &renderBuffer, uint32_t color, const Vec2<int> &p0, const Vec2<int> &p1, const Vec2<int> &p2);

	void ClearScreen(const RenderBuffer &renderBuffer, uint32_t color);

	unsigned int GetColorFromRGB(int red, int green, int blue);

	template<typename T>
	void TransformAndRenderMesh(const RenderBuffer &renderBuffer, const Mesh<T> &mesh, const Camera<T> &camera, const Matrix4x4<T> transformMatrix, const Matrix4x4<T> projectionMatrix);
}

#endif
