#ifndef GENTLE_GIANT_WIN32_H
#define GENTLE_GIANT_WIN32_H

#include <windows.h>

namespace gentle
{
	struct WindowSettings
	{
		int width;
		int height;
		char* title;
	};

	int Win32Main(HINSTANCE instance);

	int Win32Main(HINSTANCE instance, const WindowSettings &settings);
}

#endif
