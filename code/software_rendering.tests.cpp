#include "software_rendering.hpp"
#include <assert.h>

const uint32_t EMPTY = 0x000000;
const uint32_t FILLED = 0xFFFFFF;


void ClearPixelArray(uint32_t* pixelArray, int arrayLength)
{
	for (int i = 0; i < arrayLength; i += 1)
	{
		pixelArray[i] = EMPTY;
	}
}

void ClearPixelAndDepthArray(uint32_t* pixelArray, float* depthArray, int arrayLength)
{
	for (int i = 0; i < arrayLength; i += 1)
	{
		pixelArray[i] = EMPTY;
		depthArray[i] = 0.0f;
	}
}

void RunLineDrawTest(gentle::Vec2<int> p0, gentle::Vec2<int> p1, uint32_t* expectedPixels)
{
	uint32_t pixelArray[18] = { EMPTY };	// define pixels as an an array of 16 uint32_t values
											// NB this array lives on the stack in the scope of the RunSoftwareRenderingTests function only.
											// The array is sized greater than the RenderBuffer pixel array so it can pick up illegal memory writes to the pixel array
	ClearPixelArray(pixelArray, 18);

	/**
	 * Set the RenderBuffer to be a 4x4 grid of pixels (pixel ordinals 0 - 3)
	 *
	 *	    0   1   2   3
	 *	  |---|---|---|---|-0
	 *	0 |   |   |   |   |
	 *	  |---|---|---|---|-1
	 *	1 |   |   |   |   |
	 *	  |---|---|---|---|-2
	 *	2 |   |   |   |   |
	 *	  |---|---|---|---|-3
	 *	3 |   |   |   |   |
	 *	  |---|---|---|---|-4
	 *    0   1   2   3   4
	 */
	RenderBuffer renderBuffer;
	renderBuffer.height = 4;
	renderBuffer.width = 4;					// Size the buffer to 16 pixels. pixelArray is 18 pixels so the test can tell if the function ever oversteps the bounds of RenderBuffer.
	renderBuffer.pixels = &pixelArray[1];	// Use the second element in pixelArray so we can tell if the zero-th element ever gets accessed.

	gentle::DrawLineInPixels(renderBuffer, FILLED, p0, p1);

	assert(pixelArray[0] == EMPTY);	// Should NEVER get written to

	for (int i = 0; i < renderBuffer.height * renderBuffer.width; i += 1)
	{
		assert(pixelArray[i + 1] == expectedPixels[i]);
	}

	assert(pixelArray[17] == EMPTY);	// Should NEVER get written to
}

void Run4x4FillTriangleTest(gentle::Vec3<int> p0, gentle::Vec3<int> p1, gentle::Vec3<int> p2, uint32_t* expectedPixels)
{
	uint32_t pixelArray[18] = { EMPTY };	// define pixels as an an array of 16 uint32_t values
											// NB this array lives on the stack in the scope of the RunSoftwareRenderingTests function only.
											// The array is sized greater than the RenderBuffer pixel array so it can pick up illegal memory writes to the pixel array
	float depthArray[18] = { 0.0f };		// define a depth array also
	ClearPixelAndDepthArray(pixelArray, depthArray, 18);

	/**
	 * Set the RenderBuffer to be a 4x4 grid of pixels (pixel ordinals 0 - 3)
	 *
	 *	    0   1   2   3
	 *	  |---|---|---|---|-0
	 *	0 |   |   |   |   |
	 *	  |---|---|---|---|-1
	 *	1 |   |   |   |   |
	 *	  |---|---|---|---|-2
	 *	2 |   |   |   |   |
	 *	  |---|---|---|---|-3
	 *	3 |   |   |   |   |
	 *	  |---|---|---|---|-4
	 *    0   1   2   3   4
	 */
	RenderBuffer renderBuffer;
	renderBuffer.height = 4;
	renderBuffer.width = 4;					// Size the buffer to 16 pixels. pixelArray is 18 pixels so the test can tell if the function ever oversteps the bounds of RenderBuffer.
	renderBuffer.pixels = &pixelArray[1];	// Use the second element in pixelArray so we can tell if the zero-th element ever gets accessed.
	renderBuffer.depth = &depthArray[1];	// Use the second element in depthArray so we can tell if the zero-th element ever gets accessed.

	gentle::FillTriangleInPixels(renderBuffer, FILLED, p0, p1, p2, 1.0f);

	assert(pixelArray[0] == EMPTY);	// Should NEVER get written to

	for (int i = 0; i < renderBuffer.height * renderBuffer.width; i += 1)
	{
		assert(pixelArray[i + 1] == expectedPixels[i]);
	}

	assert(pixelArray[17] == EMPTY);	// Should NEVER get written to
}

void Run6x4FillTriangleTest(gentle::Vec3<int> p0, gentle::Vec3<int> p1, gentle::Vec3<int> p2, uint32_t* expectedPixels)
{
	uint32_t pixelArray[26] = { EMPTY };	// define pixels as an an array of 26 uint32_t values
											// NB this array lives on the stack in the scope of the RunSoftwareRenderingTests function only.
											// The array is sized greater than the RenderBuffer pixel array so it can pick up illegal memory writes to the pixel array
	float depthArray[26] = { 0.0f };
	ClearPixelAndDepthArray(pixelArray, depthArray, 26);

	/**
	 * Set the RenderBuffer to be a 5x5 grid of pixels (pixel ordinals 0 - 4)
	 *
	 *	    0   1   2   3   4   5
	 *	  |---|---|---|---|---|---|-0
	 *	0 |   |   |   |   |   |   |
	 *	  |---|---|---|---|---|---|-1
	 *	1 |   |   |   |   |   |   |
	 *	  |---|---|---|---|---|---|-2
	 *	2 |   |   |   |   |   |   |
	 *	  |---|---|---|---|---|---|-3
	 *	3 |   |   |   |   |   |   |
	 *	  |---|---|---|---|---|---|-4
	 *    0   1   2   3   4   5   6
	 */
	RenderBuffer renderBuffer;
	renderBuffer.height = 4;
	renderBuffer.width = 6;					// Size the buffer to 16 pixels. pixelArray is 25 pixels so the test can tell if the function ever oversteps the bounds of RenderBuffer.
	renderBuffer.pixels = &pixelArray[1];	// Use the second element in pixelArray so we can tell if the zero-th element ever gets accessed.
	renderBuffer.depth = &depthArray[1];

	gentle::FillTriangleInPixels(renderBuffer, FILLED, p0, p1, p2, 1.0f);

	assert(pixelArray[0] == EMPTY);	// Should NEVER get written to

	for (int i = 0; i < renderBuffer.height * renderBuffer.width; i += 1)
	{
		assert(pixelArray[i + 1] == expectedPixels[i]);
	}

	assert(pixelArray[25] == EMPTY);	// Should NEVER get written to
}

void RunSoftwareRenderingTests()
{
	/**
	 * STRAIGHT HORIZONTAL LINE TO BOUNDARY OF BUFFER
	 *
	 *	    0   1   2   3
	 *	  |---|---|---|---|
	 *	0 |   |   |   |   |
	 *	  |---|---|---|---|
	 *	1 |   |   |   |   |
	 *	  |---|---|---|---|
	 *	2 | O | x | x | O |
	 *	  |---|---|---|---|
	 *	3 |   |   |   |   |
	 *	  |---|---|---|---|
	 */
	uint32_t el1[16] = {
		EMPTY,	EMPTY,	EMPTY,	EMPTY,
		EMPTY,	EMPTY,	EMPTY,	EMPTY,
		FILLED,	FILLED,	FILLED,	FILLED,
		EMPTY,	EMPTY,	EMPTY,	EMPTY
	};
	RunLineDrawTest(gentle::Vec2<int>{ 0, 2 }, gentle::Vec2<int>{ 3, 2 }, el1);
	RunLineDrawTest(gentle::Vec2<int>{ 3, 2 }, gentle::Vec2<int>{ 0, 2 }, el1);

	/**
	 * STRAIGHT VERTICAL LINE TO BOUNDARY OF BUFFER
	 *
	 *	    0   1   2   3
	 *	  |---|---|---|---|
	 *	0 | O |   |   |   |
	 *	  |---|---|---|---|
	 *	1 | x |   |   |   |
	 *	  |---|---|---|---|
	 *	2 | x |   |   |   |
	 *	  |---|---|---|---|
	 *	3 | O |   |   |   |
	 *	  |---|---|---|---|
	 */
	uint32_t el2[16] = {
		FILLED,	EMPTY,	EMPTY,	EMPTY,
		FILLED,	EMPTY,	EMPTY,	EMPTY,
		FILLED,	EMPTY,	EMPTY,	EMPTY,
		FILLED,	EMPTY,	EMPTY,	EMPTY
	};
	RunLineDrawTest(gentle::Vec2<int>{ 0, 3 }, gentle::Vec2<int>{ 0, 0 }, el2);
	RunLineDrawTest(gentle::Vec2<int>{ 0, 0 }, gentle::Vec2<int>{ 0, 3 }, el2);

	/**
	 * DIAGONAL LINE TO BOUNDARY OF BUFFER
	 * X +VE INCREMENT, Y +VE INCREMENT
	 *
	 *	    0   1   2   3
	 *	  |---|---|---|---|
	 *	0 | O |   |   |   |
	 *	  |---|---|---|---|
	 *	1 |   | x |   |   |
	 *	  |---|---|---|---|
	 *	2 |   |   | x |   |
	 *	  |---|---|---|---|
	 *	3 |   |   |   | O |
	 *	  |---|---|---|---|
	 */
	uint32_t el3[16] = {
		FILLED,	EMPTY,	EMPTY,	EMPTY,
		EMPTY,	FILLED,	EMPTY,	EMPTY,
		EMPTY,	EMPTY,	FILLED,	EMPTY,
		EMPTY,	EMPTY,	EMPTY,	FILLED
	};
	RunLineDrawTest(gentle::Vec2<int>{ 0, 0 }, gentle::Vec2<int>{ 3, 3 }, el3);
	RunLineDrawTest(gentle::Vec2<int>{ 3, 3 }, gentle::Vec2<int>{ 0, 0 }, el3);

	/**
	 * DIAGONAL LINE TO BOUNDARY OF BUFFER
	 * X +VE INCREMENT, Y -VE INCREMENT
	 *
	 *	    0   1   2   3
	 *	  |---|---|---|---|
	 *	0 |   |   |   | O |
	 *	  |---|---|---|---|
	 *	1 |   |   | x |   |
	 *	  |---|---|---|---|
	 *	2 |   | x |   |   |
	 *	  |---|---|---|---|
	 *	3 | O |   |   |   |
	 *	  |---|---|---|---|
	 */
	uint32_t el4[16] = {
		EMPTY,	EMPTY,	EMPTY,	FILLED,
		EMPTY,	EMPTY,	FILLED,	EMPTY,
		EMPTY,	FILLED,	EMPTY,	EMPTY,
		FILLED,	EMPTY,	EMPTY,	EMPTY
	};
	RunLineDrawTest(gentle::Vec2<int>{ 0, 3 }, gentle::Vec2<int>{ 3, 0 }, el4);
	RunLineDrawTest(gentle::Vec2<int>{ 3, 0 }, gentle::Vec2<int>{ 0, 3 }, el4);

	/**
	 * STEEP GRADIENT, X +VE INCREMENT, Y +VE INCREMENT
	 *
	 *	    0   1   2   3
	 *	  |---|---|---|---|
	 *	0 |   | O |   |   |
	 *	  |---|---|---|---|
	 *	1 |   | x |   |   |
	 *	  |---|---|---|---|
	 *	2 |   |   | x |   |
	 *	  |---|---|---|---|
	 *	3 |   |   | O |   |
	 *	  |---|---|---|---|
	 */
	uint32_t el5[16] = {
		EMPTY,	FILLED,	EMPTY,	EMPTY,
		EMPTY,	FILLED,	EMPTY,	EMPTY,
		EMPTY,	EMPTY,	FILLED,	EMPTY,
		EMPTY,	EMPTY,	FILLED,	EMPTY
	};
	RunLineDrawTest(gentle::Vec2<int>{ 1, 0 }, gentle::Vec2<int>{ 2, 3 }, el5);
	RunLineDrawTest(gentle::Vec2<int>{ 2, 3 }, gentle::Vec2<int>{ 1, 0 }, el5);

	/**
	 * SHALLOW GRADIENT, X +VE INCREMENT, Y +VE INCREMENT
	 *
	 *	    0   1   2   3
	 *	  |---|---|---|---|
	 *	0 |   |   |   |   |
	 *	  |---|---|---|---|
	 *	1 | O | x |   |   |
	 *	  |---|---|---|---|
	 *	2 |   |   | x | O |
	 *	  |---|---|---|---|
	 *	3 |   |   |   |   |
	 *	  |---|---|---|---|
	 */
	uint32_t el6[16] = {
		EMPTY,	EMPTY,	EMPTY,	EMPTY,
		FILLED,	FILLED,	EMPTY,	EMPTY,
		EMPTY,	EMPTY,	FILLED,	FILLED,
		EMPTY,	EMPTY,	EMPTY,	EMPTY
	};
	RunLineDrawTest(gentle::Vec2<int>{ 0, 1 }, gentle::Vec2<int>{ 3, 2 }, el6);
	RunLineDrawTest(gentle::Vec2<int>{ 3, 2 }, gentle::Vec2<int>{ 0, 1 }, el6);

	/**
	 * SHALLOW GRADIENT, X +VE INCREMENT, Y -VE INCREMENT
	 *
	 *	    0   1   2   3
	 *	  |---|---|---|---|
	 *	0 |   |   |   |   |
	 *	  |---|---|---|---|
	 *	1 |   |   |   |   |
	 *	  |---|---|---|---|
	 *	2 |   |   | x | O |
	 *	  |---|---|---|---|
	 *	3 | O | x |   |   |
	 *	  |---|---|---|---|
	 */
	uint32_t el7[16] = {
		EMPTY,	EMPTY,	EMPTY,	EMPTY,
		EMPTY,	EMPTY,	EMPTY,	EMPTY,
		EMPTY,	EMPTY,	FILLED,	FILLED,
		FILLED,	FILLED,	EMPTY,	EMPTY
	};
	RunLineDrawTest(gentle::Vec2<int>{ 0, 3 }, gentle::Vec2<int>{ 3, 2 }, el7);
	RunLineDrawTest(gentle::Vec2<int>{ 3, 2 }, gentle::Vec2<int>{ 0, 3 }, el7);

	/**
	 * STEEP GRADIENT, X +VE INCREMENT, Y +VE INCREMENT
	 *
	 *	    0   1   2   3
	 *	  |---|---|---|---|
	 *	0 |   |   |   | O |
	 *	  |---|---|---|---|
	 *	1 |   |   | x |   |
	 *	  |---|---|---|---|
	 *	2 |   |   | x |   |
	 *	  |---|---|---|---|
	 *	3 |   | O |   |   |
	 *	  |---|---|---|---|
	 */
	uint32_t el8[16] = {
		EMPTY,	EMPTY,	EMPTY,	FILLED,
		EMPTY,	EMPTY,	FILLED,	EMPTY,
		EMPTY,	EMPTY,	FILLED,	EMPTY,
		EMPTY,	FILLED,	EMPTY,	EMPTY
	};
	RunLineDrawTest(gentle::Vec2<int>{ 3, 0 }, gentle::Vec2<int>{ 1, 3 }, el8);
	RunLineDrawTest(gentle::Vec2<int>{ 1, 3 }, gentle::Vec2<int>{ 3, 0 }, el8);

	/**
	 * SHORT, SHALLOW GRADIENT - RIGHT TO LEFT
	 *
	 *	    0   1   2   3
	 *	  |---|---|---|---|
	 *	0 |   |   |   | O |
	 *	  |---|---|---|---|
	 *	1 |   | O | x |   |
	 *	  |---|---|---|---|
	 *	2 |   |   |   |   |
	 *	  |---|---|---|---|
	 *	3 |   |   |   |   |
	 *	  |---|---|---|---|
	 */
	uint32_t el9[16] = {
		EMPTY,	EMPTY,	EMPTY,	FILLED,
		EMPTY,	FILLED,	FILLED,	EMPTY,
		EMPTY,	EMPTY,	EMPTY,	EMPTY,
		EMPTY,	EMPTY,	EMPTY,	EMPTY
	};
	RunLineDrawTest(gentle::Vec2<int>{ 3, 0 }, gentle::Vec2<int>{ 1, 1 }, el9);

	/**
	 * SHORT, SHALLOW GRADIENT - LEFT TO RIGHT
	 *
	 *	    0   1   2   3
	 *	  |---|---|---|---|
	 *	0 |   |   | x | O |
	 *	  |---|---|---|---|
	 *	1 |   | O |   |   |
	 *	  |---|---|---|---|
	 *	2 |   |   |   |   |
	 *	  |---|---|---|---|
	 *	3 |   |   |   |   |
	 *	  |---|---|---|---|
	 */
	uint32_t el10[16] = {
		EMPTY,	EMPTY,	FILLED,	FILLED,
		EMPTY,	FILLED,	EMPTY,	EMPTY,
		EMPTY,	EMPTY,	EMPTY,	EMPTY,
		EMPTY,	EMPTY,	EMPTY,	EMPTY
	};
	RunLineDrawTest(gentle::Vec2<int>{ 1, 1 }, gentle::Vec2<int>{ 3, 0 }, el10);

	/**
	 * SHORT, SHALLOW GRADIENT - LEFT TO RIGHT
	 *
	 *	    0   1   2   3
	 *	  |---|---|---|---|
	 *	0 |   |   |   | O |
	 *	  |---|---|---|---|
	 *	1 |   | x | x |   |
	 *	  |---|---|---|---|
	 *	2 | O |   |   |   |
	 *	  |---|---|---|---|
	 *	3 |   |   |   |   |
	 *	  |---|---|---|---|
	 */
	uint32_t el11[16] = {
		EMPTY,	EMPTY,	EMPTY,	FILLED,
		EMPTY,	FILLED,	FILLED,	EMPTY,
		FILLED,	EMPTY,	EMPTY,	EMPTY,
		EMPTY,	EMPTY,	EMPTY,	EMPTY
	};
	RunLineDrawTest(gentle::Vec2<int>{ 0, 2 }, gentle::Vec2<int>{ 3, 0 }, el11);


	/**
	 * FLAT TOP RIGHT HAND SIDE RIGHT ANGLED TRIANGLE
	 *
	 *	    0   1   2   3
	 *	  |---|---|---|---|
	 *	0 | O | x | x | O |
	 *	  |---|---|---|---|
	 *	1 |   | x | x | x |
	 *	  |---|---|---|---|
	 *	2 |   |   | x | x |
	 *	  |---|---|---|---|
	 *	3 |   |   |   | O |
	 *	  |---|---|---|---|
	 */
	uint32_t et1[16] = {
		FILLED,	FILLED,	FILLED,	FILLED,
		EMPTY,	FILLED,	FILLED,	FILLED,
		EMPTY,	EMPTY,	FILLED,	FILLED,
		EMPTY,	EMPTY,	EMPTY,	FILLED
	};
	Run4x4FillTriangleTest(gentle::Vec3<int>{ 0, 0, 0 }, gentle::Vec3<int>{ 3, 0, 0 }, gentle::Vec3<int>{ 3, 3, 0 }, et1);
	Run4x4FillTriangleTest(gentle::Vec3<int>{ 3, 3, 0 }, gentle::Vec3<int>{ 0, 0, 0 }, gentle::Vec3<int>{ 3, 0, 0 }, et1);
	Run4x4FillTriangleTest(gentle::Vec3<int>{ 3, 0, 0 }, gentle::Vec3<int>{ 3, 3, 0 }, gentle::Vec3<int>{ 0, 0, 0 }, et1);

	/**
	 * FLAT TOP LEFT HAND SIDE RIGHT ANGLED TRIANGLE
	 *
	 *	    0   1   2   3
	 *	  |---|---|---|---|
	 *	0 | O | x | x | O |
	 *	  |---|---|---|---|
	 *	1 | x | x | x |   |
	 *	  |---|---|---|---|
	 *	2 | x | x |   |   |
	 *	  |---|---|---|---|
	 *	3 | O |   |   |   |
	 *	  |---|---|---|---|
	 */
	uint32_t et2[16] = {
		FILLED,	FILLED,	FILLED,	FILLED,
		FILLED,	FILLED,	FILLED,	EMPTY,
		FILLED,	FILLED,	EMPTY,	EMPTY,
		FILLED,	EMPTY,	EMPTY,	EMPTY
	};
	Run4x4FillTriangleTest(gentle::Vec3<int>{ 0, 0, 0 }, gentle::Vec3<int>{ 3, 0, 0 }, gentle::Vec3<int>{ 0, 3, 0 }, et2);
	Run4x4FillTriangleTest(gentle::Vec3<int>{ 0, 3, 0 }, gentle::Vec3<int>{ 0, 0, 0 }, gentle::Vec3<int>{ 3, 0, 0 }, et2);
	Run4x4FillTriangleTest(gentle::Vec3<int>{ 3, 0, 0 }, gentle::Vec3<int>{ 0, 3, 0 }, gentle::Vec3<int>{ 0, 0, 0 }, et2);

	/**
	 * FLAT TOP TRIANGLE
	 *
	 *	    0   1   2   3
	 *	  |---|---|---|---|
	 *	0 | O | x | x | O |
	 *	  |---|---|---|---|
	 *	1 |   | x | x | x |
	 *	  |---|---|---|---|
	 *	2 |   | x | x |   |
	 *	  |---|---|---|---|
	 *	3 |   |   | O |   |
	 *	  |---|---|---|---|
	 */
	uint32_t et3[16] = {
		FILLED,	FILLED,	FILLED,	FILLED,
		EMPTY,	FILLED,	FILLED,	FILLED,
		EMPTY,	FILLED,	FILLED,	EMPTY,
		EMPTY,	EMPTY,	FILLED,	EMPTY
	};
	Run4x4FillTriangleTest(gentle::Vec3<int>{ 0, 0, 0 }, gentle::Vec3<int>{ 3, 0, 0 }, gentle::Vec3<int>{ 2, 3, 0 }, et3);
	Run4x4FillTriangleTest(gentle::Vec3<int>{ 2, 3, 0 }, gentle::Vec3<int>{ 0, 0, 0 }, gentle::Vec3<int>{ 3, 0, 0 }, et3);
	Run4x4FillTriangleTest(gentle::Vec3<int>{ 3, 0, 0 }, gentle::Vec3<int>{ 2, 3, 0 }, gentle::Vec3<int>{ 0, 0, 0 }, et3);

	/**
	 * FLAT BOTTOM RIGHT HAND SIDE RIGHT ANGLED TRIANGLE
	 *
	 *	    0   1   2   3
	 *	  |---|---|---|---|
	 *	0 |   |   |   | O |
	 *	  |---|---|---|---|
	 *	1 |   |   | x | x |
	 *	  |---|---|---|---|
	 *	2 |   | x | x | x |
	 *	  |---|---|---|---|
	 *	3 | 0 | x | x | O |
	 *	  |---|---|---|---|
	 */
	uint32_t et4[16] = {
		EMPTY,	EMPTY,	EMPTY,	FILLED,
		EMPTY,	EMPTY,	FILLED,	FILLED,
		EMPTY,	FILLED,	FILLED,	FILLED,
		FILLED,	FILLED,	FILLED,	FILLED
	};
	Run4x4FillTriangleTest(gentle::Vec3<int>{ 0, 3, 0 }, gentle::Vec3<int>{ 3, 0, 0 }, gentle::Vec3<int>{ 3, 3, 0 }, et4);
	Run4x4FillTriangleTest(gentle::Vec3<int>{ 3, 3, 0 }, gentle::Vec3<int>{ 0, 3, 0 }, gentle::Vec3<int>{ 3, 0, 0 }, et4);
	Run4x4FillTriangleTest(gentle::Vec3<int>{ 3, 0, 0 }, gentle::Vec3<int>{ 3, 3, 0 }, gentle::Vec3<int>{ 0, 3, 0 }, et4);

	/**
	 * FLAT BOTTOM LEFT HAND SIDE RIGHT ANGLED TRIANGLE
	 *
	 *	    0   1   2   3
	 *	  |---|---|---|---|
	 *	0 | O |   |   |   |
	 *	  |---|---|---|---|
	 *	1 | x | x |   |   |
	 *	  |---|---|---|---|
	 *	2 | x | x | x |   |
	 *	  |---|---|---|---|
	 *	3 | O | x | x | O |
	 *	  |---|---|---|---|
	 */
	uint32_t et5[16] = {
		FILLED,	EMPTY,	EMPTY,	EMPTY,
		FILLED,	FILLED,	EMPTY,	EMPTY,
		FILLED,	FILLED,	FILLED,	EMPTY,
		FILLED,	FILLED,	FILLED,	FILLED
	};
	Run4x4FillTriangleTest(gentle::Vec3<int>{ 0, 0, 0 }, gentle::Vec3<int>{ 3, 3, 0 }, gentle::Vec3<int>{ 0, 3, 0 }, et5);
	Run4x4FillTriangleTest(gentle::Vec3<int>{ 0, 3, 0 }, gentle::Vec3<int>{ 0, 0, 0 }, gentle::Vec3<int>{ 3, 3, 0 }, et5);
	Run4x4FillTriangleTest(gentle::Vec3<int>{ 3, 3, 0 }, gentle::Vec3<int>{ 0, 3, 0 }, gentle::Vec3<int>{ 0, 0, 0 }, et5);

	/**
	 * FLAT BOTTOM TRIANGLE
	 *
	 *	    0   1   2   3
	 *	  |---|---|---|---|
	 *	0 |   |   | O |   |
	 *	  |---|---|---|---|
	 *	1 |   | x | x |   |
	 *	  |---|---|---|---|
	 *	2 |   | x | x | x |
	 *	  |---|---|---|---|
	 *	3 | O | x | x | O |
	 *	  |---|---|---|---|
	 */
	uint32_t et6[16] = {
		EMPTY,	EMPTY,	FILLED,	EMPTY,
		EMPTY,	FILLED,	FILLED,	EMPTY,
		EMPTY,	FILLED,	FILLED,	FILLED,
		FILLED,	FILLED,	FILLED,	FILLED
	};
	Run4x4FillTriangleTest(gentle::Vec3<int>{ 2, 0, 0 }, gentle::Vec3<int>{ 3, 3, 0 }, gentle::Vec3<int>{ 0, 3, 0 }, et6);
	Run4x4FillTriangleTest(gentle::Vec3<int>{ 0, 3, 0 }, gentle::Vec3<int>{ 2, 0, 0 }, gentle::Vec3<int>{ 3, 3, 0 }, et6);
	Run4x4FillTriangleTest(gentle::Vec3<int>{ 3, 3, 0 }, gentle::Vec3<int>{ 0, 3, 0 }, gentle::Vec3<int>{ 2, 0, 0 }, et6);

	/**
	 * RIGHT MAJOR TRIANGLE
	 *
	 *	    0   1   2   3
	 *	  |---|---|---|---|
	 *	0 |   |   | O |   |
	 *	  |---|---|---|---|
	 *	1 |   | x | x |   |
	 *	  |---|---|---|---|
	 *	2 | O | x | x | x |
	 *	  |---|---|---|---|
	 *	3 |   |   | x | O |
	 *	  |---|---|---|---|
	 */
	uint32_t et7[16] = {
		EMPTY,	EMPTY,	FILLED,	EMPTY,
		EMPTY,	FILLED,	FILLED,	EMPTY,
		FILLED,	FILLED,	FILLED,	FILLED,
		EMPTY,	EMPTY,	FILLED,	FILLED
	};
	Run4x4FillTriangleTest(gentle::Vec3<int>{ 2, 0, 0 }, gentle::Vec3<int>{ 3, 3, 0 }, gentle::Vec3<int>{ 0, 2, 0 }, et7);
	Run4x4FillTriangleTest(gentle::Vec3<int>{ 0, 2, 0 }, gentle::Vec3<int>{ 2, 0, 0 }, gentle::Vec3<int>{ 3, 3, 0 }, et7);
	Run4x4FillTriangleTest(gentle::Vec3<int>{ 3, 3, 0 }, gentle::Vec3<int>{ 0, 2, 0 }, gentle::Vec3<int>{ 2, 0, 0 }, et7);

	/**
	 * LEFT MAJOR TRIANGLE
	 *
	 *	    0   1   2   3
	 *	  |---|---|---|---|
	 *	0 |   |   | O |   |
	 *	  |---|---|---|---|
	 *	1 |   | x | x | x |
	 *	  |---|---|---|---|
	 *	2 |   | x | x | O |
	 *	  |---|---|---|---|
	 *	3 | O | x |   |   |
	 *	  |---|---|---|---|
	 */
	uint32_t et8[16] = {
		EMPTY,	EMPTY,	FILLED,	EMPTY,
		EMPTY,	FILLED,	FILLED,	FILLED,
		EMPTY,	FILLED,	FILLED,	FILLED,
		FILLED,	FILLED,	EMPTY,	EMPTY
	};
	Run4x4FillTriangleTest(gentle::Vec3<int>{ 2, 0, 0 }, gentle::Vec3<int>{ 3, 2, 0 }, gentle::Vec3<int>{ 0, 3, 0 }, et8);
	Run4x4FillTriangleTest(gentle::Vec3<int>{ 0, 3, 0 }, gentle::Vec3<int>{ 2, 0, 0 }, gentle::Vec3<int>{ 3, 2, 0 }, et8);
	Run4x4FillTriangleTest(gentle::Vec3<int>{ 3, 2, 0 }, gentle::Vec3<int>{ 0, 3, 0 }, gentle::Vec3<int>{ 2, 0, 0 }, et8);

	/**
	 * NEW FLAT TOP TRIANGLE TEST - NARROW TALL 1
	 *
	 *	    0   1   2   3   4   5
	 *	  |---|---|---|---|---|---|
	 *	0 | O | x | x | O |   |   |
	 *	  |---|---|---|---|---|---|
	 *	1 |   | x | x | x |   |   |
	 *	  |---|---|---|---|---|---|
	 *	2 |   | x | x |   |   |   |
	 *	  |---|---|---|---|   |---|
	 *	3 |   |   | O |   |   |   |
	 *	  |---|---|---|---|---|---|
	 */
	uint32_t eft1[24] = {
		FILLED,	FILLED,	FILLED,	FILLED,	EMPTY,	EMPTY,
		EMPTY,	FILLED,	FILLED,	FILLED,	EMPTY,	EMPTY,
		EMPTY,	FILLED,	FILLED,	EMPTY,	EMPTY,	EMPTY,
		EMPTY,	EMPTY,	FILLED,	EMPTY,	EMPTY,	EMPTY
	};
	Run6x4FillTriangleTest(gentle::Vec3<int>{ 0, 0, 0 }, gentle::Vec3<int>{ 3, 0, 0 }, gentle::Vec3<int>{ 2, 3, 0 }, eft1);

	/**
	 * NEW FLAT TOP TRIANGLE TEST - NARROW TALL 2
	 *
	 *	    0   1   2   3   4   5
	 *	  |---|---|---|---|---|---|
	 *	0 | O | x | x | O |   |   |
	 *	  |---|---|---|---|---|---|
	 *	1 | x | x | x |   |   |   |
	 *	  |---|---|---|---|---|---|
	 *	2 |   | x | x |   |   |   |
	 *	  |---|---|---|---|---|---|
	 *	3 |   | O |   |   |   |   |
	 *	  |---|---|---|---|---|---|
	 */
	uint32_t eft2[24] = {
		FILLED,	FILLED,	FILLED,	FILLED,	EMPTY,	EMPTY,
		FILLED,	FILLED,	FILLED,	EMPTY,	EMPTY,	EMPTY,
		EMPTY,	FILLED,	FILLED,	EMPTY,	EMPTY,	EMPTY,
		EMPTY,	FILLED,	EMPTY,	EMPTY,	EMPTY,	EMPTY
	};
	Run6x4FillTriangleTest(gentle::Vec3<int>{ 0, 0, 0 }, gentle::Vec3<int>{ 3, 0, 0 }, gentle::Vec3<int>{ 1, 3, 0 }, eft2);

	/**
	 * NEW FLAT TOP TRIANGLE TEST - SHORT WIDE
	 *
	 *	    0   1   2   3   4   5
	 *	  |---|---|---|---|---|---|
	 *	0 | O | x | x | x | O |   |
	 *	  |---|---|---|---|---|---|
	 *	1 |   |   | O |   |   |   |
	 *	  |---|---|---|---|---|---|
	 *	2 |   |   |   |   |   |   |
	 *	  |---|---|---|---|---|---|
	 *	3 |   |   |   |   |   |   |
	 *	  |---|---|---|---|---|---|
	 */
	uint32_t eft3[24] = {
		FILLED,	FILLED,	FILLED,	FILLED,	FILLED,	EMPTY,
		EMPTY,	FILLED,	FILLED,	FILLED,	EMPTY,	EMPTY,
		EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,
		EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY
	};
	Run6x4FillTriangleTest(gentle::Vec3<int>{ 0, 0, 0 }, gentle::Vec3<int>{ 4, 0, 0 }, gentle::Vec3<int>{ 2, 1, 0 }, eft3);

	/**
	 * NEW FLAT TOP TRIANGLE TEST - SHORT WIDE
	 *
	 *	    0   1   2   3   4   5
	 *	  |---|---|---|---|---|---|
	 *	0 | O | x | x | x | x | O |
	 *	  |---|---|---|---|---|---|
	 *	1 |   | x | x | x | x |   |
	 *	  |---|---|---|---|---|---|
	 *	2 |   |   | O |   |   |   |
	 *	  |---|---|---|---|---|---|
	 *	3 |   |   |   |   |   |   |
	 *	  |---|---|---|---|---|---|
	 */
	uint32_t eft4[24] = {
		FILLED,	FILLED,	FILLED,	FILLED,	FILLED,	FILLED,
		EMPTY,	FILLED,	FILLED,	FILLED,	FILLED,	EMPTY,
		EMPTY,	EMPTY,	FILLED,	EMPTY,	EMPTY,	EMPTY,
		EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY
	};
	Run6x4FillTriangleTest(gentle::Vec3<int>{ 0, 0, 0 }, gentle::Vec3<int>{ 5, 0, 0 }, gentle::Vec3<int>{ 2, 2, 0 }, eft4);

	/**
	 * NEW FLAT TOP TRIANGLE TEST - MIXED GRADIENTS
	 *
	 *	    0   1   2   3   4   5
	 *	  |---|---|---|---|---|---|
	 *	0 | O | x | x | x | x | O |
	 *	  |---|---|---|---|---|---|
	 *	1 |   | x | x | x | x |   |
	 *	  |---|---|---|---|---|---|
	 *	2 |   | O | x |   |   |   |
	 *	  |---|---|---|---|---|---|
	 *	3 |   |   |   |   |   |   |
	 *	  |---|---|---|---|---|---|
	 */
	uint32_t eft5[24] = {
		FILLED,	FILLED,	FILLED,	FILLED,	FILLED,	FILLED,
		EMPTY,	FILLED,	FILLED,	FILLED,	FILLED,	EMPTY,
		EMPTY,	FILLED,	FILLED,	EMPTY,	EMPTY,	EMPTY,
		EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY
	};
	Run6x4FillTriangleTest(gentle::Vec3<int>{ 0, 0, 0 }, gentle::Vec3<int>{ 5, 0, 0 }, gentle::Vec3<int>{ 1, 2, 0 }, eft5);

	/**
	 * NEW FLAT TOP TRIANGLE TEST - MIXED GRADIENTS 2
	 *
	 *	    0   1   2   3   4   5
	 *	  |---|---|---|---|---|---|
	 *	0 | O | x | x | x | x | O |
	 *	  |---|---|---|---|---|---|
	 *	1 |   | x | x | x | x |   |
	 *	  |---|---|---|---|---|---|
	 *	2 |   |   |   | x | O |   |
	 *	  |---|---|---|---|---|---|
	 *	3 |   |   |   |   |   |   |
	 *	  |---|---|---|---|---|---|
	 */
	uint32_t eft6[24] = {
		FILLED,	FILLED,	FILLED,	FILLED,	FILLED,	FILLED,
		EMPTY,	FILLED,	FILLED,	FILLED,	FILLED,	EMPTY,
		EMPTY,	EMPTY,	EMPTY,	FILLED,	FILLED,	EMPTY,
		EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY
	};
	Run6x4FillTriangleTest(gentle::Vec3<int>{ 0, 0, 0 }, gentle::Vec3<int>{ 5, 0, 0 }, gentle::Vec3<int>{ 4, 2, 0 }, eft6);

	/**
	 * NEW FLAT TOP TRIANGLE TEST - RIGHT HAND RIGHT ANGLE
	 *
	 *	    0   1   2   3   4   5
	 *	  |---|---|---|---|---|---|
	 *	0 | O | x | x | x | x | O |
	 *	  |---|---|---|---|---|---|
	 *	1 |   | x | x | x | x | x |
	 *	  |---|---|---|---|---|---|
	 *	2 |   |   |   | x | x | x |
	 *	  |---|---|---|---|---|---|
	 *	3 |   |   |   |   |   | O |
	 *	  |---|---|---|---|---|---|
	 */
	uint32_t eft7[24] = {
		FILLED,	FILLED,	FILLED,	FILLED,	FILLED,	FILLED,
		EMPTY,	FILLED,	FILLED,	FILLED,	FILLED,	FILLED,
		EMPTY,	EMPTY,	EMPTY,	FILLED,	FILLED,	FILLED,
		EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	FILLED
	};
	Run6x4FillTriangleTest(gentle::Vec3<int>{ 0, 0, 0 }, gentle::Vec3<int>{ 5, 0, 0 }, gentle::Vec3<int>{ 5, 3, 0 }, eft7);

	/**
	 * NEW FLAT TOP TRIANGLE TEST - LEFT HAND RIGHT ANGLE
	 *
	 *	    0   1   2   3   4   5
	 *	  |---|---|---|---|---|---|
	 *	0 | O | x | x | x | x | O |
	 *	  |---|---|---|---|---|---|
	 *	1 | x | x | x | x | x |   |
	 *	  |---|---|---|---|---|---|
	 *	2 | x | x | x |   |   |   |
	 *	  |---|---|---|---|---|---|
	 *	3 | O |   |   |   |   |   |
	 *	  |---|---|---|---|---|---|
	 */
	uint32_t eft8[24] = {
		FILLED,	FILLED,	FILLED,	FILLED,	FILLED,	FILLED,
		FILLED,	FILLED,	FILLED,	FILLED,	FILLED,	EMPTY,
		FILLED,	FILLED,	FILLED,	EMPTY,	EMPTY,	EMPTY,
		FILLED,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY
	};
	Run6x4FillTriangleTest(gentle::Vec3<int>{ 0, 0, 0 }, gentle::Vec3<int>{ 5, 0, 0 }, gentle::Vec3<int>{ 0, 3, 0 }, eft8);

	/**
	 * NEW FLAT TOP TRIANGLE TEST - SKEWED LEFT
	 *
	 *	    0   1   2   3   4   5
	 *	  |---|---|---|---|---|---|
	 *	0 |   |   | O | x | x | O |
	 *	  |---|---|---|---|---|---|
	 *	1 |   | x | x | x | x |   |
	 *	  |---|---|---|---|---|---|
	 *	2 |   | x | x |   |   |   |
	 *	  |---|---|---|---|---|---|
	 *	3 | O |   |   |   |   |   |
	 *	  |---|---|---|---|---|---|
	 */
	uint32_t eft9[24] = {
		EMPTY,	EMPTY,	FILLED,	FILLED,	FILLED,	FILLED,
		EMPTY,	FILLED,	FILLED,	FILLED,	FILLED,	EMPTY,
		EMPTY,	FILLED,	FILLED,	EMPTY,	EMPTY,	EMPTY,
		FILLED,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY
	};
	Run6x4FillTriangleTest(gentle::Vec3<int>{ 2, 0, 0 }, gentle::Vec3<int>{ 5, 0, 0 }, gentle::Vec3<int>{ 0, 3, 0 }, eft9);

	/**
	 * NEW FLAT TOP TRIANGLE TEST - SKEWED RIGHT
	 *
	 *	    0   1   2   3   4   5
	 *	  |---|---|---|---|---|---|
	 *	0 | O | x | x | O |   |   |
	 *	  |---|---|---|---|---|---|
	 *	1 |   | x | x | x | x |   |
	 *	  |---|---|---|---|---|---|
	 *	2 |   |   |   | x | x |   |
	 *	  |---|---|---|---|---|---|
	 *	3 |   |   |   |   |   | O |
	 *	  |---|---|---|---|---|---|
	 */
	uint32_t eft10[24] = {
		FILLED,	FILLED,	FILLED,	FILLED,	EMPTY,	EMPTY,
		EMPTY,	FILLED,	FILLED,	FILLED,	FILLED,	EMPTY,
		EMPTY,	EMPTY,	EMPTY,	FILLED,	FILLED,	EMPTY,
		EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	FILLED
	};
	Run6x4FillTriangleTest(gentle::Vec3<int>{ 0, 0, 0 }, gentle::Vec3<int>{ 3, 0, 0 }, gentle::Vec3<int>{ 5, 3, 0 }, eft10);

	/**
	 * NEW FLAT BOTTOM TRIANGLE TEST - NARROW TALL 1
	 *
	 *	    0   1   2   3   4   5
	 *	  |---|---|---|---|---|---|
	 *	0 |   |   | O |   |   |   |
	 *	  |---|---|---|---|---|---|
	 *	1 |   | x | x |   |   |   |
	 *	  |---|---|---|---|---|---|
	 *	2 |   | x | x | x |   |   |
	 *	  |---|---|---|---|   |---|
	 *	3 | O | x | x | O |   |   |
	 *	  |---|---|---|---|---|---|
	 */
	uint32_t efb1[24] = {
		EMPTY,	EMPTY,	FILLED,	EMPTY,	EMPTY,	EMPTY,
		EMPTY,	FILLED,	FILLED,	EMPTY,	EMPTY,	EMPTY,
		EMPTY,	FILLED,	FILLED,	FILLED,	EMPTY,	EMPTY,
		FILLED,	FILLED,	FILLED,	FILLED,	EMPTY,	EMPTY
	};
	Run6x4FillTriangleTest(gentle::Vec3<int>{ 2, 0, 0 }, gentle::Vec3<int>{ 0, 3, 0 }, gentle::Vec3<int>{ 3, 3, 0 }, efb1);

	/**
	 * NEW FLAT BOTTOM TRIANGLE TEST - NARROW TALL 2
	 *
	 *	    0   1   2   3   4   5
	 *	  |---|---|---|---|---|---|
	 *	0 |   | O |   |   |   |   |
	 *	  |---|---|---|---|---|---|
	 *	1 |   | x | x |   |   |   |
	 *	  |---|---|---|---|---|---|
	 *	2 | x | x | x |   |   |   |
	 *	  |---|---|---|---|---|---|
	 *	3 | O | x | x | O |   |   |
	 *	  |---|---|---|---|---|---|
	 */
	uint32_t efb2[24] = {
		EMPTY,	FILLED,	EMPTY,	EMPTY,	EMPTY,	EMPTY,
		EMPTY,	FILLED,	FILLED,	EMPTY,	EMPTY,	EMPTY,
		FILLED,	FILLED,	FILLED,	EMPTY,	EMPTY,	EMPTY,
		FILLED,	FILLED,	FILLED,	FILLED,	EMPTY,	EMPTY
	};
	Run6x4FillTriangleTest(gentle::Vec3<int>{ 1, 0, 0 }, gentle::Vec3<int>{ 0, 3, 0 }, gentle::Vec3<int>{ 3, 3, 0 }, efb2);

	/**
	 * NEW FLAT BOTTOM TRIANGLE TEST - SHORT WIDE
	 *
	 *	    0   1   2   3   4   5
	 *	  |---|---|---|---|---|---|
	 *	0 |   |   | O |   |   |   |
	 *	  |---|---|---|---|---|---|
	 *	1 | O | x | x | x | O |   |
	 *	  |---|---|---|---|---|---|
	 *	2 |   |   |   |   |   |   |
	 *	  |---|---|---|---|---|---|
	 *	3 |   |   |   |   |   |   |
	 *	  |---|---|---|---|---|---|
	 */
	uint32_t efb3[24] = {
		EMPTY,	EMPTY,	FILLED,	EMPTY,	EMPTY,	EMPTY,
		FILLED,	FILLED,	FILLED,	FILLED,	FILLED,	EMPTY,
		EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,
		EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY
	};
	Run6x4FillTriangleTest(gentle::Vec3<int>{ 2, 0, 0 }, gentle::Vec3<int>{ 0, 1, 0 }, gentle::Vec3<int>{ 4, 1, 0 }, efb3);

	/**
	 * NEW FLAT BOTTOM TRIANGLE TEST - SHORT WIDE
	 *
	 *	    0   1   2   3   4   5
	 *	  |---|---|---|---|---|---|
	 *	0 |   |   | O |   |   |   |
	 *	  |---|---|---|---|---|---|
	 *	1 |   | x | x | x | x |   |
	 *	  |---|---|---|---|---|---|
	 *	2 | O | x | x | x | x | O |
	 *	  |---|---|---|---|---|---|
	 *	3 |   |   |   |   |   |   |
	 *	  |---|---|---|---|---|---|
	 */
	uint32_t efb4[24] = {
		EMPTY,	EMPTY,	FILLED,	EMPTY,	EMPTY,	EMPTY,
		EMPTY,	FILLED,	FILLED,	FILLED,	FILLED,	EMPTY,
		FILLED,	FILLED,	FILLED,	FILLED,	FILLED,	FILLED,
		EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY
	};
	Run6x4FillTriangleTest(gentle::Vec3<int>{ 2, 0, 0 }, gentle::Vec3<int>{ 0, 2, 0 }, gentle::Vec3<int>{ 5, 2, 0 }, efb4);

	/**
	 * NEW FLAT BOTTOM TRIANGLE TEST - MIXED GRADIENTS
	 *
	 *	    0   1   2   3   4   5
	 *	  |---|---|---|---|---|---|
	 *	0 |   | O |   |   |   |   |
	 *	  |---|---|---|---|---|---|
	 *	1 | x | x | x | x |   |   |
	 *	  |---|---|---|---|---|---|
	 *	2 | O | x | x | x | x | O |
	 *	  |---|---|---|---|---|---|
	 *	3 |   |   |   |   |   |   |
	 *	  |---|---|---|---|---|---|
	 */
	uint32_t efb5[24] = {
		EMPTY,	FILLED,	EMPTY,	EMPTY,	EMPTY,	EMPTY,
		FILLED,	FILLED,	FILLED,	FILLED,	EMPTY,	EMPTY,
		FILLED,	FILLED,	FILLED,	FILLED,	FILLED,	FILLED,
		EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY
	};
	Run6x4FillTriangleTest(gentle::Vec3<int>{ 1, 0, 0 }, gentle::Vec3<int>{ 0, 2, 0 }, gentle::Vec3<int>{ 5, 2, 0 }, efb5);

	/**
	 * NEW FLAT BOTTOM TRIANGLE TEST - MIXED GRADIENTS 2
	 *
	 *	    0   1   2   3   4   5
	 *	  |---|---|---|---|---|---|
	 *	0 |   |   |   |   | O |   |
	 *	  |---|---|---|---|---|---|
	 *	1 |   |   | x | x | x |   |
	 *	  |---|---|---|---|---|---|
	 *	2 | O | x | x | x | x | O |
	 *	  |---|---|---|---|---|---|
	 *	3 |   |   |   |   |   |   |
	 *	  |---|---|---|---|---|---|
	 */
	uint32_t efb6[24] = {
		EMPTY,	EMPTY,	EMPTY,	EMPTY,	FILLED,	EMPTY,
		EMPTY,	EMPTY,	FILLED,	FILLED,	FILLED,	FILLED,
		FILLED,	FILLED,	FILLED,	FILLED,	FILLED,	FILLED,
		EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY
	};
	Run6x4FillTriangleTest(gentle::Vec3<int>{ 4, 0, 0 }, gentle::Vec3<int>{ 0, 2, 0 }, gentle::Vec3<int>{ 5, 2, 0 }, efb6);

	/**
	 * NEW FLAT BOTTOM TRIANGLE TEST - RIGHT HAND RIGHT ANGLE
	 *
	 *	    0   1   2   3   4   5
	 *	  |---|---|---|---|---|---|
	 *	0 |   |   |   |   |   | O |
	 *	  |---|---|---|---|---|---|
	 *	1 |   |   |   | x | x | x |
	 *	  |---|---|---|---|---|---|
	 *	2 |   | x | x | x | x | x |
	 *	  |---|---|---|---|---|---|
	 *	3 | O | x | x | x | x | O |
	 *	  |---|---|---|---|---|---|
	 */
	uint32_t efb7[24] = {
		EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	FILLED,
		EMPTY,	EMPTY,	EMPTY,	FILLED,	FILLED,	FILLED,
		EMPTY,	FILLED,	FILLED,	FILLED,	FILLED,	FILLED,
		FILLED,	FILLED,	FILLED,	FILLED,	FILLED,	FILLED
	};
	Run6x4FillTriangleTest(gentle::Vec3<int>{ 5, 0, 0 }, gentle::Vec3<int>{ 0, 3, 0 }, gentle::Vec3<int>{ 5, 3, 0 }, efb7);

	/**
	 * NEW FLAT BOTTOM TRIANGLE TEST - LEFT HAND RIGHT ANGLE
	 *
	 *	    0   1   2   3   4   5
	 *	  |---|---|---|---|---|---|
	 *	0 | O |   |   |   |   |   |
	 *	  |---|---|---|---|---|---|
	 *	1 | x | x | x |   |   |   |
	 *	  |---|---|---|---|---|---|
	 *	2 | x | x | x | x | x |   |
	 *	  |---|---|---|---|---|---|
	 *	3 | O | x | x | x | x | O |
	 *	  |---|---|---|---|---|---|
	 */
	uint32_t efb8[24] = {
		FILLED,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,
		FILLED,	FILLED,	FILLED,	EMPTY,	EMPTY,	EMPTY,
		FILLED,	FILLED,	FILLED,	FILLED,	FILLED,	EMPTY,
		FILLED,	FILLED,	FILLED,	FILLED,	FILLED,	FILLED
	};
	Run6x4FillTriangleTest(gentle::Vec3<int>{ 0, 0, 0 }, gentle::Vec3<int>{ 0, 3, 0 }, gentle::Vec3<int>{ 5, 3, 0 }, efb8);

	/**
	 * NEW FLAT BOTTOM TRIANGLE TEST - SKEWED LEFT
	 *
	 *	    0   1   2   3   4   5
	 *	  |---|---|---|---|---|---|
	 *	0 | O |   |   |   |   |   |
	 *	  |---|---|---|---|---|---|
	 *	1 |   | x | x |   |   |   |
	 *	  |---|---|---|---|---|---|
	 *	2 |   | x | x | x | x |   |
	 *	  |---|---|---|---|---|---|
	 *	3 |   |   | O | x | x | O |
	 *	  |---|---|---|---|---|---|
	 */
	uint32_t efb9[24] = {
		FILLED,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,
		EMPTY,	FILLED,	FILLED,	EMPTY,	EMPTY,	EMPTY,
		EMPTY,	FILLED,	FILLED,	FILLED,	FILLED,	EMPTY,
		EMPTY,	EMPTY,	FILLED,	FILLED,	FILLED,	FILLED
	};
	Run6x4FillTriangleTest(gentle::Vec3<int>{ 0, 0, 0 }, gentle::Vec3<int>{ 2, 3, 0 }, gentle::Vec3<int>{ 5, 3, 0 }, efb9);

	/**
	 * NEW FLAT BOTTOM TRIANGLE TEST - SKEWED RIGHT
	 *
	 *	    0   1   2   3   4   5
	 *	  |---|---|---|---|---|---|
	 *	0 |   |   |   |   |   | O |
	 *	  |---|---|---|---|---|---|
	 *	1 |   |   |   | x | x |   |
	 *	  |---|---|---|---|---|---|
	 *	2 |   | x | x | x | x |   |
	 *	  |---|---|---|---|---|---|
	 *	3 | O | x | x | O |   |   |
	 *	  |---|---|---|---|---|---|
	 */
	uint32_t efb10[24] = {
		EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	FILLED,
		EMPTY,	EMPTY,	EMPTY,	FILLED,	FILLED,	EMPTY,
		EMPTY,	FILLED,	FILLED,	FILLED,	FILLED,	EMPTY,
		FILLED,	FILLED,	FILLED,	FILLED,	EMPTY,	EMPTY
	};
	Run6x4FillTriangleTest(gentle::Vec3<int>{ 5, 0, 0 }, gentle::Vec3<int>{ 0, 3, 0 }, gentle::Vec3<int>{ 3, 3, 0 }, efb10);
}