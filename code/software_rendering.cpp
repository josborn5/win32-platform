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
	 * x1, x2 & y parameters are the pixel and NOT the position ordinals
	 */
	static void DrawHorizontalLineInPixels(const RenderBuffer &renderBuffer, uint32_t color, int x0, int x1, int y)
	{
		const int* startX = &x0;
		const int* endX = &x1;
		if (x1 < x0)
		{
			std::swap(x0, x1);
		}

		int positionStartOfRow = renderBuffer.width * y;
		int positionOfX0InRow = positionStartOfRow + *startX;
		uint32_t* pixelPointer = renderBuffer.pixels + positionOfX0InRow;
		for (int i = *startX; i <= *endX; i += 1)
		{
			*pixelPointer = color;
			pixelPointer++;
		}
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
		bool negativeXDiff = (xDiff < 0);
		bool negativeYDiff = (yDiff < 0);
		int xDiffMod = (negativeXDiff) ? -1 * xDiff : xDiff;
		int yDiffMod = (negativeYDiff) ? -1 * yDiff : yDiff;
		int xIncrement = (negativeXDiff) ? -1 : 1;
		int yIncrement = (negativeYDiff) ? -1 : 1;

		// If the gradient is 1 simply increment both X & Y at on every iteration
		if (xDiffMod == yDiffMod)
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
		int longDimensionDiff;
		int* longDimensionVar;	// Make this a pointer so PlotPixel can still be called with x0 & y0 arguments
		int longDimensionIncrement;
		int shortDimensionDiff;
		int* shortDimensionVar;	// Make this a pointer so PlotPixel can still be called with x0 & y0 arguments
		int shortDimensionIncrement;

		if (yDiffMod < xDiffMod)
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

		int negativePIncrement = 2 * shortDimensionDiff;
		int p = negativePIncrement - longDimensionDiff;
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

	/*	p0------p1
	 *	\       /	|
	 *	 \     /	|
	 *	  \   /		V
	 *	   \ /	  +ve y (if +ve y is up, this is actually a flat bottom triangle)
	 *	    p2
	 */
	void FillFlatTopTriangleZwei(const RenderBuffer &renderBuffer, uint32_t color, const math::Vec3<int> &p0, const math::Vec3<int> &p1, const math::Vec3<int> &p2)
	{
		// LINE 0-->2
		bool p2IsRightOfP0 = (p0.x < p2.x);
		int xDiff0 = (p2IsRightOfP0) ? p2.x - p0.x : p0.x - p2.x;
		int yDiff0 = p2.y - p0.y;

		bool isLongDimension0X = (yDiff0 < xDiff0);
		int longDelta0 = (isLongDimension0X) ? xDiff0 : yDiff0;
		int shortDelta0 = (isLongDimension0X) ? yDiff0 : xDiff0;

		int negIncrement0 = 2 * shortDelta0;
		int acc0 = negIncrement0 - longDelta0;
		int posIncrement0 = negIncrement0 - (2 * longDelta0);
		int x0Increment = (p2IsRightOfP0) ? 1 : -1;

		// LINE 1-->2
		// Vertical distance for 1-->2 is the same as 0-->2, so no need for a separate yDiff1 variable. Can reuse yDiff0.
		bool p2IsLeftOfP1 = (p2.x < p1.x);
		int xDiff1 = (p2IsLeftOfP1) ? p1.x - p2.x : p1.x - p2.x;

		bool isLongDimension1X = (yDiff0 < xDiff1);
		int longDelta1 = (isLongDimension1X) ? xDiff1 : yDiff0;
		int shortDelta1 = (isLongDimension1X) ? yDiff0 : xDiff1;

		int negIncrement1 = 2 * shortDelta1;
		int acc1 = negIncrement1 - longDelta1;
		int posIncrement1 = negIncrement1 - (2 * longDelta1);
		int x1Increment = (p2IsLeftOfP1) ? -1 : 1;

		// Copy the x & y values for p0 & p1 so we can modify them safely inside this function
		// Note that p0.y == p1.y so we only need one variable for the y position
		int x0 = p0.x;
		int x1 = p1.x;
		for (int y = p0.y; y <= p2.y; y += 1)
		{
			// Loop through the x0 / acc0 evaluation until acc0 is +ve.
			// acc0 turning +ve is the indication we should plot.
			if (isLongDimension0X)
			{
				while ((acc0 < 0) && (negIncrement0 > 0)) // x0 needs to increment AT LEAST once since it's the long dimension. i.e. it needs to increment more than y
				{
					acc0 += negIncrement0;
					x0 += x0Increment;
				}
			}

			if (isLongDimension1X)
			{
				while ((acc1 < 0) && (negIncrement1 > 0))
				{
					acc1 += negIncrement1;
					x1 += x1Increment;
				}
			}

			// draw scanline to fill in triangle between x0 & x1
			DrawHorizontalLineInPixels(renderBuffer, color, x0, x1, y);

			// line p0 --> p2: decide to increment x0 or not for next y
			if (isLongDimension0X) // X0 needs to increment AT LEAST once since it's the long dimension. i.e. it needs to increment more than y
			{
				acc0 += posIncrement0;
				x0 += x0Increment;
			}
			else
			{
				if (acc0 < 0)
				{
					acc0 += negIncrement0;
				}
				else
				{
					acc0 += posIncrement0;
					x0 += x0Increment;
				}
			}

			// line p1 --> p2: decide to decrement x1 or not for next y
			if (isLongDimension1X)
			{
				acc1 += posIncrement1;
				x1 += x1Increment;
			}
			else
			{
				if (acc1 < 0)
				{
					acc1 += negIncrement1;
				}
				else
				{
					acc1 += posIncrement1;
					x1 += x1Increment;
				}
			}
		}
	}

	/*	p0------p1
	 *	\       /	|
	 *	 \     /	|
	 *	  \   /		V
	 *	   \ /	  +ve y (if +ve y is up, this is actually a flat bottom triangle)
	 *	    p2
	 */
	void FillFlatTopTriangle(const RenderBuffer &renderBuffer, uint32_t color, const math::Vec3<int> &p0, const math::Vec3<int> &p1, const math::Vec3<int> &p2)
	{
		if ((p0.x == p1.x && p0.y == p1.y) || (p0.x == p2.x && p0.y == p2.y) || (p1.x == p2.x && p1.y == p2.y))
		{
			// TODO: figure out why condition is occurring - is it a float --> int rounding thing when the triangle gets split???
			return;
		}
		// Slopes
		float m0 = (float)(p2.x - p0.x) / (float)(p2.y - p0.y);
		float m1 = (float)(p2.x - p1.x) / (float)(p2.y - p1.y);

		// step along the y-axis, drawing a horizontal scanline at each y ordinal
		for (int y = p0.y; y <= p2.y; y += 1)
		{
			// TODO: can I use the line drawing algorithm here instead?
			const int px0 = (int)std::floor(m0 * (float)(y - p0.y) + (float)p0.x);
			const int px1 = (int)std::ceil(m1 * (float)(y - p1.y) + (float)p1.x);

			// x: start & end of scanline
			for (int x = px0; x <= px1; x += 1)
			{
				PlotPixel(renderBuffer, color, x, y);
			}
		}
	}

	/*`     p0
	 *`     /\			|
	 *`    /  \			|
	 *`   /    \		V
	 *	 /      \	  +ve y (if +ve y is up, this is actually a flat top triangle)
	 *	p1------p2
	 */
	void FillFlatBottomTriangleZwei(const RenderBuffer &renderBuffer, uint32_t color, const math::Vec3<int> &p0, const math::Vec3<int> &p1, const math::Vec3<int> &p2)
	{
		// LINE 0-->1
		bool p1IsLeftOfP0 = (p1.x < p0.x);
		int xDiff0 = (p1IsLeftOfP0) ? p0.x - p1.x : p1.x - p0.x;
		int yDiff0 = p1.y - p0.y;

		bool isLongDimension0X = (yDiff0 < xDiff0);
		int longDelta0 = (isLongDimension0X) ? xDiff0 : yDiff0;
		int shortDelta0 = (isLongDimension0X) ? yDiff0 : xDiff0;

		int negIncrement0 = 2 * shortDelta0;
		int acc0 = negIncrement0 - longDelta0;
		int posIncrement0 = negIncrement0 - (2 * longDelta0);
		int x0Increment = (p1IsLeftOfP0) ? -1 : 1;

		// LINE 0-->2
		// Vertical distance for 1-->2 is the same as 0-->2, so no need for a separate yDiff1 variable. Can reuse yDiff0.
		bool p2IsRightOfP0 = (p0.x < p2.x);
		int xDiff1 = (p2IsRightOfP0) ? p2.x - p0.x : p0.x - p2.x;

		bool isLongDimension1X = (yDiff0 < xDiff1);
		int longDelta1 = (isLongDimension1X) ? xDiff1 : yDiff0;
		int shortDelta1 = (isLongDimension1X) ? yDiff0 : xDiff1;

		int negIncrement1 = 2 * shortDelta1;
		int acc1 = negIncrement1 - longDelta1;
		int posIncrement1 = negIncrement1 - (2 * longDelta1);
		int x1Increment = (p2IsRightOfP0) ? 1 : -1;

		// Copy the x & y values for p0 & p1 so we can modify them safely inside this function
		// Note that p0.y == p1.y so we only need one variable for the y position
		int x0 = p0.x;
		int x1 = p0.x;

		for (int y = p0.y; y <= p1.y; y += 1)
		{
			// Loop through the x0 / acc0 evaluation until acc0 is +ve.
			// acc0 turning +ve is the indication we should plot.
			if (isLongDimension0X)
			{
				while ((acc0 < 0) && (negIncrement0 > 0)) // x0 needs to increment AT LEAST once since it's the long dimension. i.e. it needs to increment more than y
				{
					acc0 += negIncrement0;
					x0 += x0Increment;
				}
			}

			if (isLongDimension1X)
			{
				while ((acc1 < 0) && (negIncrement1 > 0))
				{
					acc1 += negIncrement1;
					x1 += x1Increment;
				}
			}

			// draw scanline to fill in triangle between x0 & x1
			DrawHorizontalLineInPixels(renderBuffer, color, x0, x1, y);

			// line p0 --> p1: decide to increment x0 or not for current y
			if (isLongDimension0X)
			{
				acc0 += posIncrement0;
				x0 += x0Increment;
			}
			else
			{
				if (acc0 < 0)
				{
					acc0 += negIncrement0;
				}
				else
				{
					acc0 += posIncrement0;
					x0 += x0Increment;
				}
			}

			// line p0 --> p2: decide to decrement x1 or not for next y
			if (isLongDimension1X)
			{
				acc1 += posIncrement1;
				x1 += x1Increment;
			}
			else
			{
				if (acc1 < 0)
				{
					acc1 += negIncrement1;
				}
				else
				{
					acc1 += posIncrement1;
					x1 += x1Increment;
				}
			}
		}

		// Draw final scanline
		// DrawHorizontalLineInPixels(renderBuffer, color, p1.x, p2.x, p1.y);
	}

	/*`     p0
	 *`     /\			|
	 *`    /  \			|
	 *`   /    \		V
	 *	 /      \	  +ve y (if +ve y is up, this is actually a flat top triangle)
	 *	p1------p2
	 */
	void FillFlatBottomTriangle(const RenderBuffer &renderBuffer, uint32_t color, const math::Vec3<int> &p0, const math::Vec3<int> &p1, const math::Vec3<int> &p2)
	{
		if ((p0.x == p1.x && p0.y == p1.y) || (p0.x == p2.x && p0.y == p2.y) || (p1.x == p2.x && p1.y == p2.y))
		{
			// TODO: figure out why condition is occurring - is it a float --> int rounding thing when the triangle gets split???
			return;
		}
		// Slopes
		float m2 = (float)(p2.x - p0.x) / (float)(p2.y - p0.y);
		float m1 = (float)(p1.x - p0.x) / (float)(p1.y - p0.y);

		for (int y = p0.y; y <= p2.y; y += 1)
		{
			const int px2 = (int)std::floor(m2 * (float)(y - p2.y) + (float)p2.x);
			const int px1 = (int)std::ceil(m1 * (float)(y - p1.y) + (float)p1.x);

			for (int x = px1; x <= px2; x += 1)
			{
				PlotPixel(renderBuffer, color, x, y);
			}
		}
	}

	void FillTriangleInPixels(const RenderBuffer &renderBuffer, uint32_t color, const math::Vec3<int> &p0, const math::Vec3<int> &p1, const math::Vec3<int> &p2)
	{
		const math::Vec3<int>* pp0 = &p0;
		const math::Vec3<int>* pp1 = &p1;
		const math::Vec3<int>* pp2 = &p2;

		/* Sort the three points of the triangle by their y co-ordinate
		 *
		 * pp0->y	low		|
		 * pp1->y			|
		 * pp2->y	high	V
		 *				  +ve y
		 */
		if (pp1->y < pp0->y)
		{
			std::swap(pp0, pp1);
		}
		if (pp2->y < pp1->y)
		{
			std::swap(pp1, pp2);
		}
		if (pp1->y < pp0->y)
		{
			std::swap(pp0, pp1);
		}

		// Check for natural flat top
		if (pp0 -> y == pp1->y)
		{
			// sort top two points of flat top by their x co-ordinate
			if (pp1->x < pp0->x)
			{
				std::swap(pp0, pp1);
			}
			FillFlatTopTriangle(renderBuffer, color, *pp0, *pp1, *pp2);
		}
		else if (pp1->y == pp2->y) // natural flat bottom
		{
			// sort bottom two points of flat bottom by their x co-ordinate
			if (pp2->x < pp1->x)
			{
				std::swap(pp1, pp2);
			}
			FillFlatBottomTriangle(renderBuffer, color, *pp0, *pp1, *pp2);
		}
		else // general triangle - need to split it in two
		{
			const float splitFactor = (float)(pp1->y - pp0->y) / (float)(pp2->y - pp0->y);
			const math::Vec3<int> diff = math::SubtractVectors(*pp2, *pp0);
			const math::Vec3<float> toSplit = math::MultiplyVectorByScalar(math::Vec3<float>{ (float)diff.x, (float)diff.y, (float)diff.z }, splitFactor);
			const math::Vec3<int> splitPoint = math::AddVectors(*pp0, math::Vec3<int>{ (int)toSplit.x, (int)toSplit.y, (int)toSplit.z });

			if (pp1->x < splitPoint.x)	// major right triangle
			{
				/*				p0
				 *			   / |
				 *	|		  /  |
				 *	|		p1---| split
				 *	V		  \  |
				 * +ve y	   \ |
				 *				p2
				 */
				FillFlatBottomTriangle(renderBuffer, color, *pp0, *pp1, splitPoint);
				FillFlatTopTriangle(renderBuffer, color, *pp1, splitPoint, *pp2);
			}
			else if (splitPoint.x < pp1->x)	// major left triangle
			{
				/*			  p0
				 *			  | \
				 *	|		  |  \
				 *	|	split |---p1
				 *	V		  |  /
				 * +ve y	  | /
				 *			  p2
				 */
				FillFlatBottomTriangle(renderBuffer, color, *pp0, splitPoint, *pp1); // !!! TODO - figure out why this can cause ~8000ms rendering times for the teapot !!!
				FillFlatTopTriangle(renderBuffer, color, splitPoint, *pp1, *pp2);
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

	template<typename T>
	void TransformAndRenderMesh(const RenderBuffer &renderBuffer, const Mesh<T> &mesh, const Camera<T> &camera, const math::Matrix4x4<T> transformMatrix)
	{
		const int RED = 0;
		const int GREEN = 255;
		const int BLUE = 0;

		// Camera matrix
		math::Vec4<T> target = AddVectors(camera.position, camera.direction);
		math::Matrix4x4<float> cameraMatrix = PointAt(camera.position, target, camera.up);

		// View matrix
		math::Matrix4x4<float> viewMatrix = LookAt(cameraMatrix);

		std::vector<Triangle4d<float>> trianglesToDraw;

		for (Triangle4d<float> tri : mesh.triangles)
		{
			Triangle4d<float> transformed;
			Triangle4d<float> viewed;
			Triangle4d<float> projected; // TODO: switch this to Triangle4d so the depth information is kept and can be used in a depth buffer to prevent double rendering of triangles behind each other

			// Transform the triangle in the mesh
			math::MultiplyVectorWithMatrix(tri.p[0], transformed.p[0], transformMatrix);
			math::MultiplyVectorWithMatrix(tri.p[1], transformed.p[1], transformMatrix);
			math::MultiplyVectorWithMatrix(tri.p[2], transformed.p[2], transformMatrix);

			// Work out the normal of the triangle
			math::Vec4<float> line1 = SubtractVectors(transformed.p[1], transformed.p[0]);
			math::Vec4<float> line2 = SubtractVectors(transformed.p[2], transformed.p[0]);
			math::Vec4<float> normal = math::UnitVector(math::CrossProduct(line1, line2));

			math::Vec4<float> fromCameraToTriangle = math::SubtractVectors(transformed.p[0], camera.position);
			float dot = DotProduct(normal, fromCameraToTriangle);

			if (dot >= 0.0f)
			{
				math::Vec4<float> lightDirection = { 0.0f, 0.0f, 1.0f };
				math::Vec4<float> normalizedLightDirection = UnitVector(lightDirection);
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
				Triangle4d<float> clipped[2];
				Plane<float> inFrontOfScreen = { 0.0f, 0.0f, 0.1f,	 0.0f, 0.0f, 1.0f };
				int clippedTriangleCount = ClipTriangleAgainstPlane(inFrontOfScreen, viewed, clipped[0], clipped[1]);

				for (int i = 0; i < clippedTriangleCount; i += 1)
				{
					// Project each triangle in 3D space onto the 2D space triangle to render
					math::Project3DPointTo2D(clipped[i].p[0], projected.p[0], projectionMatrix);
					math::Project3DPointTo2D(clipped[i].p[1], projected.p[1], projectionMatrix);
					math::Project3DPointTo2D(clipped[i].p[2], projected.p[2], projectionMatrix);

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
				// math::Vec2<int> p0Int = { (int)draw.p[0].x, (int)draw.p[0].y };
				// math::Vec2<int> p1Int = { (int)draw.p[1].x, (int)draw.p[1].y };
				// math::Vec2<int> p2Int = { (int)draw.p[2].x, (int)draw.p[2].y };
				// render::DrawTriangleInPixels(renderBuffer, draw.color, p0Int, p1Int, p2Int);

				math::Vec3<int> p0Int = { (int)draw.p[0].x, (int)draw.p[0].y };
				math::Vec3<int> p1Int = { (int)draw.p[1].x, (int)draw.p[1].y };
				math::Vec3<int> p2Int = { (int)draw.p[2].x, (int)draw.p[2].y };
				render::FillTriangleInPixels(renderBuffer, draw.color, p0Int, p1Int, p2Int);
			}
		}
	}
}