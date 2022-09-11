#include <windows.h>
#include "../gentle_giant_win32.hpp"

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR commandLine, int showCode)
{
	gentle::WindowSettings settings;
	settings.title = "Demo";
	settings.width = 1280;
	settings.height = 720;

	gentle::Win32Main(instance, settings);
}