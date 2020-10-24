#include "software_rendering.cpp"
#include <assert.h>

const uint32_t EMPTY = 0x000000;
const uint32_t FILLED = 0xFFFFFF;

void ClearPixels(uint32_t* pixel)
{
	for (int i = 0; i < 18; i += 1)
	{
		*pixel = EMPTY;
		pixel++;
	}
}

void RunSoftwareRenderingTests()
{
	uint32_t pixelArray[18] = { EMPTY };	// define pixels as an an array of 16 uint32_t values
											// NB this array lives on the stack in the scope of the RunSoftwareRenderingTests function only.
											// The array is sized greater than the RenderBuffer pixel array so it can pick up illegal memory writes to the pixel array

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
	ClearPixels(pixelArray);
	render::DrawLineInPixels(renderBuffer, FILLED, math::Vec2<int>{ 0, 2 }, math::Vec2<int>{ 3, 2 });

	assert(pixelArray[0] == EMPTY);	// Should NEVER get written to

	assert(pixelArray[1] == EMPTY);
	assert(pixelArray[2] == EMPTY);
	assert(pixelArray[3] == EMPTY);
	assert(pixelArray[4] == EMPTY);

	assert(pixelArray[5] == EMPTY);
	assert(pixelArray[6] == EMPTY);
	assert(pixelArray[7] == EMPTY);
	assert(pixelArray[8] == EMPTY);

	assert(pixelArray[9] == FILLED);
	assert(pixelArray[10] == FILLED);
	assert(pixelArray[11] == FILLED);
	assert(pixelArray[12] == FILLED);

	assert(pixelArray[13] == EMPTY);
	assert(pixelArray[14] == EMPTY);
	assert(pixelArray[15] == EMPTY);
	assert(pixelArray[16] == EMPTY);

	assert(pixelArray[17] == EMPTY);	// Should NEVER get written to

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
	 ClearPixels(pixelArray);
	render::DrawLineInPixels(renderBuffer, FILLED, math::Vec2<int>{ 0, 3 }, math::Vec2<int>{ 0, 0 });

	assert(pixelArray[0] == EMPTY);	// Should NEVER get written to

	assert(pixelArray[1] == FILLED);
	assert(pixelArray[2] == EMPTY);
	assert(pixelArray[3] == EMPTY);
	assert(pixelArray[4] == EMPTY);

	assert(pixelArray[5] == FILLED);
	assert(pixelArray[6] == EMPTY);
	assert(pixelArray[7] == EMPTY);
	assert(pixelArray[8] == EMPTY);

	assert(pixelArray[9] == FILLED);
	assert(pixelArray[10] == EMPTY);
	assert(pixelArray[11] == EMPTY);
	assert(pixelArray[12] == EMPTY);

	assert(pixelArray[13] == FILLED);
	assert(pixelArray[14] == EMPTY);
	assert(pixelArray[15] == EMPTY);
	assert(pixelArray[16] == EMPTY);

	assert(pixelArray[17] == EMPTY);	// Should NEVER get written to

	/**
	 * DIAGONAL LINE TO BOUNDARY OF BUFFER
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
	 ClearPixels(pixelArray);
	render::DrawLineInPixels(renderBuffer, FILLED, math::Vec2<int>{ 0, 0 }, math::Vec2<int>{ 3, 3 });

	assert(pixelArray[0] == EMPTY);	// Should NEVER get written to

	assert(pixelArray[1] == FILLED);
	assert(pixelArray[2] == EMPTY);
	assert(pixelArray[3] == EMPTY);
	assert(pixelArray[4] == EMPTY);

	assert(pixelArray[5] == EMPTY);
	assert(pixelArray[6] == FILLED);
	assert(pixelArray[7] == EMPTY);
	assert(pixelArray[8] == EMPTY);

	assert(pixelArray[9] == EMPTY);
	assert(pixelArray[10] == EMPTY);
	assert(pixelArray[11] == FILLED);
	assert(pixelArray[12] == EMPTY);

	assert(pixelArray[13] == EMPTY);
	assert(pixelArray[14] == EMPTY);
	assert(pixelArray[15] == EMPTY);
	assert(pixelArray[16] == FILLED);

	assert(pixelArray[17] == EMPTY);	// Should NEVER get written to
}