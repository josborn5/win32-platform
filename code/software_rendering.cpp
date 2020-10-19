#include "platform.hpp"
#include "math.hpp"

namespace render
{
	static int ClampInt(int min, int val, int max)
	{
		if (val < min) return min;
		if (val > max) return max;
		return val;
	}

	static void PlotPixel(RenderBuffer* renderBuffer, uint32_t color, int x, int y)
	{
		int positionStartOfRow = renderBuffer->width * y;
		int positionStartOfX0InRow = positionStartOfRow + x;
		uint32_t* pixel = renderBuffer->pixels + positionStartOfX0InRow;
		*pixel = color;
	}

	static void DrawVerticalLineInPixels(RenderBuffer* renderBuffer, uint32_t color, int x, int y0, int y1)
	{
		int yDiff = y1 - y0;
		int yDiffMod = (yDiff < 0) ? -1 * yDiff : yDiff;
		int yIncrement = (yDiff < 0) ? -1 : 1;
		PlotPixel(renderBuffer, color, x, y0);
		for (int i = 0; i <= yDiffMod; i += 1)
		{
			y0 += yIncrement;
			PlotPixel(renderBuffer, color, x, y0);
		}
	}

	static void DrawHorizontalLineInPixels(RenderBuffer* renderBuffer, uint32_t color, int x0, int x1, int y)
	{
		int xDiff = x1 - x0;
		int xDiffMod = (xDiff < 0) ? -1 * xDiff : xDiff;
		int xIncrement = (xDiff < 0) ? -1 : 1;
		PlotPixel(renderBuffer, color, x0, y);
		for (int i = 0; i < xDiffMod; i += 1)
		{
			x0 += xIncrement;
			PlotPixel(renderBuffer, color, x0, y);
		}
	}

	/**
	 *	|---|---|---|
	 *	0   1   2   3	position ordinals
	 *	  1   2   3		pixel ordinals
	 */
	// Implemented with Bresenham's algorithm
	static void DrawLineInPixels(RenderBuffer* renderBuffer, uint32_t color, math::Vec2<int> p0, math::Vec2<int> p1)
	{
		// Make sure writing to the render buffer does not escape its bounds
		int x0 = ClampInt(1, p0.x, renderBuffer->xMax);
		int y0 = ClampInt(1, p0.y, renderBuffer->yMax);
		int x1 = ClampInt(1, p1.x, renderBuffer->xMax);
		int y1 = ClampInt(1, p1.y, renderBuffer->yMax);
		
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

		PlotPixel(renderBuffer, color, x0, y0);
		// If the gradient is 1 simply increment both X & Y at on every iteration
		if (modDiff == 0)
		{
			for (int i = 0; i < xDiffMod; ++i)
			{
				x0 += xIncrement;
				y0 += yIncrement;
				PlotPixel(renderBuffer, color, x0, y0);
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
;			longDimensionIncrement = xIncrement;

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

		int p = (2 * xDiffMod) - yDiffMod;
		int negativePIncrement = 2 * shortDimensionDiff;
		int positivePIncrement = negativePIncrement - (2 * longDimensionDiff);

		for (int i = 1; i < longDimensionDiff; i += 1)
		{
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
			PlotPixel(renderBuffer, color, x0, y0);
		}
	}
}