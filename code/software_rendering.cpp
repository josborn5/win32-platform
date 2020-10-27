#include "platform.hpp"
#include "math.hpp"
#include "geometry.hpp"

namespace render
{
	/*static int ClampInt(int min, int val, int max)
	{
		if (val < min) return min;
		if (val > max) return max;
		return val;
	}*/

	/**
	 *	|---|---|---|
	 *	| 0 | 1 | 2 |	pixel ordinals
	 *	|---|---|---|
	 *	0   1   2   3	position ordinals
	 *
	 * x & y parameters are the pixel and NOT the position ordinals
	 */
	static void PlotPixel(const RenderBuffer &renderBuffer, uint32_t color, int x, int y)
	{
		// Make sure writing to the render buffer does not escape its bounds
		if (x < 0 || x >(renderBuffer.width - 1) || y < 0 || y >(renderBuffer.height - 1))
		{
			return;
		}

		int positionStartOfRow = renderBuffer.width * y;
		int positionStartOfX0InRow = positionStartOfRow + x;
		uint32_t* pixel = renderBuffer.pixels + positionStartOfX0InRow;
		*pixel = color;
	}

	/**
	 *	|---|---|---|
	 *	| 0 | 1 | 2 |	pixel ordinals
	 *	|---|---|---|
	 *	0   1   2   3	position ordinals
	 *
	 * x, y0 & y1 parameters are the pixel and NOT the position ordinals
	 */
	static void DrawVerticalLineInPixels(const RenderBuffer &renderBuffer, uint32_t color, int x, int y0, int y1)
	{
		int yDiff = y1 - y0;
		int yDiffMod = (yDiff < 0) ? -1 * yDiff : yDiff;
		int yIncrement = (yDiff < 0) ? -1 : 1;
		for (int i = 0; i <= yDiffMod; i += 1)
		{
			PlotPixel(renderBuffer, color, x, y0);
			y0 += yIncrement;
		}
	}

	/**
	 *	|---|---|---|
	 *	| 0 | 1 | 2 |	pixel ordinals
	 *	|---|---|---|
	 *	0   1   2   3	position ordinals
	 *
	 * x1, x2 & y parameters are the pixel and NOT the position ordinals
	 */
	static void DrawHorizontalLineInPixels(const RenderBuffer &renderBuffer, uint32_t color, int x0, int x1, int y)
	{
		int xDiff = x1 - x0;
		int xDiffMod = (xDiff < 0) ? -1 * xDiff : xDiff;
		int xIncrement = (xDiff < 0) ? -1 : 1;

		for (int i = 0; i <= xDiffMod; i += 1)
		{
			PlotPixel(renderBuffer, color, x0, y);
			x0 += xIncrement;
		}
	}

	/**
	 *	|---|---|---|
	 *	| 0 | 1 | 2 |	pixel ordinals
	 *	|---|---|---|
	 *	0   1   2   3	position ordinals
	 *
	 * p0 & p1 are pixel and NOT position ordinals
	 */
	// Implemented with Bresenham's algorithm
	static void DrawLineInPixels(const RenderBuffer &renderBuffer, uint32_t color, const math::Vec2<int> &p0, const math::Vec2<int> &p1)
	{
		// Make sure writing to the render buffer does not escape its bounds
		int x0 = p0.x;
		int y0 = p0.y;
		int x1 = p1.x;
		int y1 = p1.y;
		
		int xDiff = x1 - x0;
		if (xDiff == 0)
		{
			DrawVerticalLineInPixels(renderBuffer, color, x0, y0, y1);
			return;
		}

		int yDiff = y1 - y0;
		if (yDiff == 0)
		{
			DrawHorizontalLineInPixels(renderBuffer, color, x0, x1, y0);
			return;
		}

		int xDiffMod = (xDiff < 0) ? -1 * xDiff : xDiff;
		int yDiffMod = (yDiff < 0) ? -1 * yDiff : yDiff;
		int modDiff = yDiffMod - xDiffMod;
		int xIncrement = (xDiff < 0) ? -1 : 1;
		int yIncrement = (yDiff < 0) ? -1 : 1;

		// If the gradient is 1 simply increment both X & Y at on every iteration
		if (modDiff == 0)
		{
			for (int i = 0; i <= xDiffMod; ++i)
			{
				PlotPixel(renderBuffer, color, x0, y0);
				x0 += xIncrement;
				y0 += yIncrement;
			}
			return;
		}

		// If the gradient is more than one then y gets incremented on every step along the line and x sometimes gets incremented
		// If the gradient is less than one then x gets incremented on every step along the line and y sometimes gets incremented
		bool isLongDimensionX = (modDiff < 0);
		int longDimensionDiff;
		int* longDimensionVar;	// Make this a pointer so PlotPixel can still be called with x0 & y0 arguments
		int longDimensionIncrement;
		int shortDimensionDiff;
		int* shortDimensionVar;	// Make this a pointer so PlotPixel can still be called with x0 & y0 arguments
		int shortDimensionIncrement;

		if (isLongDimensionX)
		{
			longDimensionDiff = xDiffMod;
			longDimensionVar = &x0;
			longDimensionIncrement = xIncrement;

			shortDimensionDiff = yDiffMod;
			shortDimensionVar = &y0;
			shortDimensionIncrement = yIncrement;
		}
		else
		{
			longDimensionDiff = yDiffMod;
			longDimensionVar = &y0;
			longDimensionIncrement = yIncrement;

			shortDimensionDiff = xDiffMod;
			shortDimensionVar = &x0;
			shortDimensionIncrement = xIncrement;
		}

		int p = (2 * shortDimensionDiff) - longDimensionDiff;
		int negativePIncrement = 2 * shortDimensionDiff;
		int positivePIncrement = negativePIncrement - (2 * longDimensionDiff);

		for (int i = 0; i <= longDimensionDiff; i += 1)
		{
			PlotPixel(renderBuffer, color, x0, y0);
			*longDimensionVar += longDimensionIncrement;
			if (p < 0)
			{
				p += negativePIncrement;
			}
			else
			{
				p += positivePIncrement;
				*shortDimensionVar += shortDimensionIncrement;
			}
		}
	}

	void DrawTriangleInPixels(const RenderBuffer &renderBuffer, uint32_t color, const math::Vec2<int> &p0, const math::Vec2<int> &p1, const math::Vec2<int> &p2)
	{
		DrawLineInPixels(renderBuffer, color, p0, p1);
		DrawLineInPixels(renderBuffer, color, p1, p2);
		DrawLineInPixels(renderBuffer, color, p2, p0);
	}

	void ClearScreen(const RenderBuffer &renderBuffer, int x0, int y0, int x1, int y1, uint32_t color)
	{
		uint32_t *pixel = renderBuffer.pixels;

		for (int y = y0; y < y1; y += 1)
		{
			for (int x = x0; x< x1; x += 1)
			{
				*pixel = color;
				pixel++;
			}
		}
	}
}