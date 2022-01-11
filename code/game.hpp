#ifndef GENTLE_GAME_H
#define GENTLE_GAME_H

namespace gentle
{
	void Initialize(const GameMemory &gameMemory, const RenderBuffer &renderBuffer);
	void UpdateAndRender(const GameMemory &gameMemory, const Input &input, const RenderBuffer &renderBuffer, float dt);
}

#endif