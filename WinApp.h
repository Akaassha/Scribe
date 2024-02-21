#pragma once
#include <Windows.h>

class WinApp
{
public:
	WinApp(HINSTANCE instance);
	~WinApp() {};

	static void Run();
	static HWND& GetHandle();

private:
	static void make_window_class(WNDCLASS& winClass);
	static void make_handle_to_window(HWND& HanleToWindow);

	static LRESULT CALLBACK WinProc(HWND win, UINT msg, WPARAM wp, LPARAM lp);
	static void ProcessMessages(void);

	static int fullscreenWidth;
	static int fullscreenHeight;

	static bool bRun;
};