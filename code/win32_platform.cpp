#ifndef WIN32_PLATFORM_H
#define WIN32_PLATFORM_H

#include <windows.h>
#include <stdint.h>
#include <stdio.h>

#include "platform.hpp"

void GameInitialize(const GameMemory &gameMemory, const RenderBuffer &renderBuffer);
void GameUpdateAndRender(const GameMemory &gameMemory, const Input &input, const RenderBuffer &renderBuffer, float dt);

namespace platform
{

#define DEBUG_BUFFER_SIZE 256

/**
 * FLAG_SLOW
 * 0 - No slow code allowed
 * 1 - Slow code welcome
 */
#if FLAG_SLOW
	#define Assert(expression) if (!(expression)) { *(int*)0 = 0; }
#else
	#define Assert(expression)
#endif
#define Kilobytes(value) ((value) * 1024LL)
#define Megabytes(value) (Kilobytes(value) * 1024LL)
#define Gigabytes(value) (Megabytes(value) * 1024LL)
#define Terabytes(value) (Gigabytes(value) * 1024LL)

static bool IsRunning = false;
static RenderBuffer renderBuffer = {0};
static BITMAPINFO bitmapInfo = {0};	// platform dependent
static int64_t GlobalPerfCountFrequency;

static void Win32_SizeRenderBufferToCurrentWindow(HWND window)
{
	RECT clientRect = {0};
	GetClientRect(window, &clientRect);

	renderBuffer.width = clientRect.right - clientRect.left;
	renderBuffer.height = clientRect.bottom - clientRect.top;
	renderBuffer.bytesPerPixel = 4;

	if (renderBuffer.pixels)
	{
		VirtualFree(renderBuffer.pixels, 0, MEM_RELEASE);
	}

	bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
	bitmapInfo.bmiHeader.biWidth = renderBuffer.width;
	bitmapInfo.bmiHeader.biHeight = renderBuffer.height;
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = 32;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;

	int bitmapMemorySize = (renderBuffer.width * renderBuffer.height) * renderBuffer.bytesPerPixel;
	renderBuffer.pitch = renderBuffer.width * renderBuffer.bytesPerPixel;
	renderBuffer.pixels = (uint32_t *)VirtualAlloc(0, bitmapMemorySize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
}

static void Win32_DisplayRenderBufferInWindow(HDC deviceContext)
{
	StretchDIBits(deviceContext,
		0, 0, renderBuffer.width, renderBuffer.height,
		0, 0, renderBuffer.width, renderBuffer.height,
		renderBuffer.pixels, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK Win32_MainWindowCallback(HWND window, UINT Message, WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = -1;

	switch(Message)
	{
		case WM_SIZE:
		{
			Win32_SizeRenderBufferToCurrentWindow(window);
		} break;
		case WM_DESTROY:
		{
			IsRunning = false;
		} break;
		case WM_CLOSE:
		{
			IsRunning = false;
		} break;
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			Assert(!"NOOOOOOOO");
		} break;
		case WM_PAINT:
		{
			PAINTSTRUCT paint = {0};
			HDC deviceContext = BeginPaint(window, &paint);
			Win32_DisplayRenderBufferInWindow(deviceContext);
			EndPaint(window, &paint);
		} break;
		default:
		{
			// use windows default callback handler
			Result = DefWindowProc(window, Message, wParam, lParam);
		} break;
	}

	return Result;
}

static void Win32_ProcessKeyboardMessage(Input* input, int isDown, int wasDown, int vkCode, int vkButton, KEY key)
{
	if (vkCode == vkButton)
	{
		Button button = input->buttons[key];
		button.isDown = (isDown != 0);
		button.wasDown = (wasDown != 0);
		button.keyUp = (!button.isDown && button.wasDown);
	}
}

static void ResetButtons(Input *gameInput)
{
	for (int i = 0; i < BUTTON_COUNT; i += 1)
	{
		if (gameInput->buttons[i].keyUp)
		{
			gameInput->buttons[i].wasDown = false;
			gameInput->buttons[i].keyUp = false;
		}
	}
}

static void Win32_ProcessPendingMessages(Input* input)
{
	// flush the queue of Messages from windows in this loop
	MSG Message;
	while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
	{
		if(Message.message == WM_QUIT)
		{
			IsRunning = false;
		}

		switch(Message.message)
		{
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
			case WM_KEYDOWN:
			case WM_KEYUP:
			{
				uint32_t vKCode = (uint32_t)Message.wParam;
				bool wasDown = ((Message.lParam & (1 << 30)) != 0); // Bit #30 of the LParam tells us what the previous key was
				bool isDown = ((Message.lParam & (1 << 31)) == 0); // Bit #31 of the LParam tells us what the current key is
				if (wasDown != isDown)
				{
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'A', KEY_A);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'B', KEY_B);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'C', KEY_C);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'D', KEY_D);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'E', KEY_E);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'F', KEY_F);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'G', KEY_G);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'H', KEY_H);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'I', KEY_I);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'J', KEY_J);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'K', KEY_K);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'L', KEY_L);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'M', KEY_M);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'N', KEY_N);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'O', KEY_O);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'P', KEY_P);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'Q', KEY_Q);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'R', KEY_R);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'S', KEY_S);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'T', KEY_T);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'U', KEY_U);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'V', KEY_V);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'W', KEY_W);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'X', KEY_X);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'Y', KEY_Y);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, 'Z', KEY_Z);

					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, '0', KEY_0);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, '1', KEY_1);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, '2', KEY_2);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, '3', KEY_3);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, '4', KEY_4);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, '5', KEY_5);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, '6', KEY_6);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, '7', KEY_7);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, '8', KEY_8);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, '9', KEY_9);

					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, VK_SPACE, KEY_SPACE);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, VK_UP, KEY_UP);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, VK_DOWN, KEY_DOWN);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, VK_LEFT, KEY_LEFT);
					Win32_ProcessKeyboardMessage(input, isDown, wasDown, vKCode, VK_RIGHT, KEY_RIGHT);
				}

				bool altKeyDown = ((Message.lParam & (1 << 29)) != 0);
				if((vKCode == VK_F4) && altKeyDown)
				{
					IsRunning = false;
				}
			} break;
			default: {
				TranslateMessage(&Message);
				DispatchMessage(&Message);
			} break;

		}
	}
}

void DisplayLastWin32Error()
{
	DWORD ErrorCode = GetLastError();
	char ErrorCodeBuffer[256];
	wsprintf(ErrorCodeBuffer, "VirtualAlloc error code: %d\n", ErrorCode);
}

inline LARGE_INTEGER Win32_GetWallClock()
{
	LARGE_INTEGER Result;
	QueryPerformanceCounter(&Result);
	return Result;
}

inline float Win32_GetSecondsElapsed(LARGE_INTEGER Start, LARGE_INTEGER End)
{
	uint64_t CounterElapsed = End.QuadPart - Start.QuadPart;
	float SecondsElapsedForWork = ((float)CounterElapsed / (float)GlobalPerfCountFrequency);
	return SecondsElapsedForWork;
}

}

// NB this needs to be outside of the namespace declaration so the linker can see it as the entry points
int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR commandLine, int showCode)
{
	LARGE_INTEGER PerfCounterFrequencyResult;
	QueryPerformanceFrequency(&PerfCounterFrequencyResult);
	platform::GlobalPerfCountFrequency = PerfCounterFrequencyResult.QuadPart;

	// Set the Windows schedular granularity to 1ms to help our Sleep() function call be granular
	UINT DesiredSchedulerMS = 1;
	MMRESULT setSchedularGranularityResult = timeBeginPeriod(DesiredSchedulerMS);
	bool SleepIsGranular = (setSchedularGranularityResult == TIMERR_NOERROR);

	WNDCLASSA windowClass = {0};
	windowClass.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
	windowClass.lpfnWndProc = platform::Win32_MainWindowCallback;
	windowClass.hInstance = instance;
	windowClass.lpszClassName = "Window Class";

	const int GameUpdateHz = 30;
	float targetSecondsPerFrame = 1.0f / (float)GameUpdateHz;

	if(RegisterClassA(&windowClass))
	{
		HWND window = CreateWindowExA(0, windowClass.lpszClassName, "Breakout Clone",
									WS_VISIBLE|WS_OVERLAPPEDWINDOW,
									CW_USEDEFAULT, CW_USEDEFAULT,
									1280, 720, 0, 0, 0, 0);
		if(window)
		{
			platform::IsRunning = true;

			// Initialize Visual
			platform::Win32_SizeRenderBufferToCurrentWindow(window);

			// Initialize general use memory
			
			GameMemory GameMemory;
			GameMemory.PermanentStorageSpace = Megabytes(1);
			GameMemory.TransientStorageSpace = Megabytes((uint64_t)1);

			uint64_t totalStorageSpace = GameMemory.PermanentStorageSpace + GameMemory.TransientStorageSpace;
			bool successfulMemoryAllocation = true;
			GameMemory.PermanentStorage = VirtualAlloc(0, (size_t)totalStorageSpace, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
			if(GameMemory.PermanentStorage == NULL)
			{
				successfulMemoryAllocation = false;
				platform::DisplayLastWin32Error();
			}

			GameMemory.TransientStorage = (uint8_t*)GameMemory.PermanentStorage + GameMemory.PermanentStorageSpace;

			// Initialize input state
			Input gameInput = {0};

			// Initialize timers
			float lastDt = targetSecondsPerFrame;
			LARGE_INTEGER LastCounter = platform::Win32_GetWallClock();
			int64_t LastCycleCount = __rdtsc();

			GameInitialize(GameMemory, platform::renderBuffer);

			// Main loop
			while (successfulMemoryAllocation && platform::IsRunning)
			{
				platform::Win32_ProcessPendingMessages(&gameInput);

				// Get mouse position
				POINT mousePointer;
				GetCursorPos(&mousePointer);	// mousePointer in screen coord
				ScreenToClient(window, &mousePointer);	// convert screen coord to window coord
				gameInput.mouse.x = mousePointer.x;
				gameInput.mouse.y = platform::renderBuffer.height - mousePointer.y;


				GameUpdateAndRender(GameMemory, gameInput, platform::renderBuffer, lastDt);


				platform::ResetButtons(&gameInput);

				// render visual
				HDC deviceContext = GetDC(window);
				platform::Win32_DisplayRenderBufferInWindow(deviceContext);
				ReleaseDC(window, deviceContext);

				// wait before starting next frame
				float secondsElapsedForFrame = platform::Win32_GetSecondsElapsed(LastCounter, platform::Win32_GetWallClock());
				float workTime = 1000.0f * secondsElapsedForFrame;
				if (secondsElapsedForFrame < targetSecondsPerFrame)
				{
					if (SleepIsGranular)
					{
						DWORD SleepMS = (DWORD)(1000.0f * (targetSecondsPerFrame - secondsElapsedForFrame));
						if (SleepMS > 0)
						{
							Sleep(SleepMS);
						}
					}
					while(secondsElapsedForFrame < targetSecondsPerFrame)
					{
						secondsElapsedForFrame = platform::Win32_GetSecondsElapsed(LastCounter, platform::Win32_GetWallClock());
					}
				}
				else
				{
					// TODO MISSED FRAME RATE
				}

				// Take end of frame measurements
				LARGE_INTEGER EndCounter = platform::Win32_GetWallClock();
				int64_t EndCycleCount = __rdtsc();

				// Work out elapsed time for current frame
				lastDt = platform::Win32_GetSecondsElapsed(LastCounter, EndCounter);

				// Output frame tine information
				uint64_t counterElapsed = LastCounter.QuadPart - EndCounter.QuadPart;
				double FPS = (double)platform::GlobalPerfCountFrequency / (double)counterElapsed;
				int64_t CyclesElapsed = EndCycleCount - LastCycleCount;
				double MCPF = (double)CyclesElapsed / (1000.0f * 1000.0f);

				char buffer[DEBUG_BUFFER_SIZE];
				float msPerFrame = 1000.0f * lastDt;
				sprintf_s(buffer, DEBUG_BUFFER_SIZE, "%.02f ms, %.02f ms/f,  %.02f f/s,  %.02f MC/f\n", workTime, msPerFrame, FPS, MCPF);
				OutputDebugStringA(buffer);

				// Reset measurementsfor next frame
				LastCounter = EndCounter;
				LastCycleCount = EndCycleCount;
			}
		}
		else
		{
			// Handle unable to create window
		}
	}
	else
	{
		// Handle windows window registration failure
	}

	return (0);
}

#endif
