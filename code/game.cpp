#include "win32_platform.cpp"
#include "software_rendering.cpp"

#include "math.hpp"

void GameInitialize()
{
}

void GameUpdateAndRender(const GameMemory &gameMemory, const Input &input, const RenderBuffer &renderBuffer, float dt)
{
	const uint32_t TEXT_COLOR = 0x00FF00;
	const uint32_t BACKGROUND_COLOR = 0x000000;

	render::ClearScreen(renderBuffer, 0, 0, renderBuffer.width, renderBuffer.height, BACKGROUND_COLOR);

	math::Vec2<int> bottomLeft = { 0, renderBuffer.height };
	math::Vec2<int> bottomRight = { renderBuffer.width, renderBuffer.height };
	math::Vec2<int> topLeft = { 0, 0 };
	math::Vec2<int> topRight = { renderBuffer.width, 0 };
	render::DrawLineInPixels(renderBuffer, TEXT_COLOR, bottomLeft, topRight);
	render::DrawLineInPixels(renderBuffer, TEXT_COLOR, bottomRight, topLeft);

	int halfHeight = (int)(renderBuffer.height / 2);
	int halfWidth = (int)(renderBuffer.width / 2);
	math::Vec2<int> middleLeft = { 0, halfHeight };
	math::Vec2<int> middleRight = { renderBuffer.width, halfHeight };
	math::Vec2<int> middleTop = { halfWidth, 0 };
	math::Vec2<int> middleBottom = { halfWidth, renderBuffer.height };
	render::DrawLineInPixels(renderBuffer, TEXT_COLOR, middleLeft, middleRight);
	render::DrawLineInPixels(renderBuffer, TEXT_COLOR, middleBottom, middleTop);
}
