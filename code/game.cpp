#include "win32_platform.cpp"
#include "software_rendering.cpp"

#include "math.hpp"

void GameUpdateAndRender(const GameMemory &gameMemory, const Input &input, const RenderBuffer &renderBuffer, float dt)
{
	const uint32_t TEXT_COLOR = 0xFFFF00;

	math::Vec2<int> bottomLeft = math::Vec2<int>(0, renderBuffer.height);
	math::Vec2<int> bottomRight = math::Vec2<int>(renderBuffer.width, renderBuffer.height);
	math::Vec2<int> topLeft = math::Vec2<int>(0, 0);
	math::Vec2<int> topRight = math::Vec2<int>(renderBuffer.width, 0);
	render::DrawLineInPixels(renderBuffer, TEXT_COLOR, bottomLeft, topRight);
	render::DrawLineInPixels(renderBuffer, TEXT_COLOR, bottomRight, topLeft);

	int halfHeight = (int)(renderBuffer.height / 2);
	int halfWidth = (int)(renderBuffer.width / 2);
	math::Vec2<int> middleLeft = math::Vec2<int>(0, halfHeight);
	math::Vec2<int> middleRight = math::Vec2<int>(renderBuffer.width, halfHeight);
	math::Vec2<int> middleTop = math::Vec2<int>(halfWidth, 0);
	math::Vec2<int> middleBottom = math::Vec2<int>(halfWidth, renderBuffer.height);
	render::DrawLineInPixels(renderBuffer, TEXT_COLOR, middleLeft, middleRight);
	render::DrawLineInPixels(renderBuffer, TEXT_COLOR, middleBottom, middleTop);
}
