#include "WinApp.h"
#include "clipboardManager.h"
#include "CroppingRect.h"
#include "OCR.h"
#include "Convert.h"
#include "opencv.hpp"
#include "Files.h"
#include <string>
#include <algorithm>

#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

static HWND WindowHandle;
static WNDCLASS WindowClass;
static HINSTANCE WindowInstance;
const LPCWSTR WIN_CLASS_NAME = TEXT("WIN_CLASS");
const LPCWSTR APP_TITLE = TEXT("APP_TITLE");
const DWORD WINDOWED_STYLE = 0;

int WinApp::fullscreenWidth = 0;
int WinApp::fullscreenHeight = 0;

bool WinApp::bRun = true;

HWND& WinApp::GetHandle() 
{ 
	return WindowHandle;
};

WinApp::WinApp(HINSTANCE instance)
{
	WindowInstance = instance;

	make_window_class(WindowClass);
	RegisterClass(&WindowClass);
	make_handle_to_window(WindowHandle);

	ShowWindow(WindowHandle, SW_SHOWNORMAL);
}

void WinApp::make_window_class(WNDCLASS& winClass)
{
	winClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	winClass.hInstance = WindowInstance;
	winClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	winClass.lpszMenuName = NULL;
	winClass.lpszClassName = WIN_CLASS_NAME;
	winClass.lpfnWndProc = &WinProc;
}

void WinApp::make_handle_to_window(HWND& HanleToWindow)
{
	HanleToWindow = CreateWindow(
		WIN_CLASS_NAME,      /*Class name*/
		APP_TITLE,           /*Window Title*/
		WINDOWED_STYLE,      /*Window Style*/
		0,                   /*X Starting pos*/
		0,                   /*Y Starting Pos*/
		600,				 /*Width*/
		400,				 /*Height*/
		0,                   /*Parent Window*/
		0,                   /*Menu*/
		WindowInstance,      /*HInstance*/
		0					 /*Lparam This will be available in WM_CREATE*/
	);
}

void WinApp::Run()
{
	HWND hwndDesktop = GetDesktopWindow();
	HDC desktopDC = GetDC(hwndDesktop);
	fullscreenWidth = GetDeviceCaps(desktopDC, DESKTOPHORZRES);
	fullscreenHeight = GetDeviceCaps(desktopDC, DESKTOPVERTRES);

	SetWindowPos(WindowHandle, HWND_NOTOPMOST, 0, 0, fullscreenWidth, fullscreenHeight, SWP_SHOWWINDOW);
	SetWindowLongPtr(WindowHandle, GWL_STYLE, WS_POPUP | WS_VISIBLE);
	
	FileManager::HDCToFile("C:\\temp\\Image.bmp", desktopDC);

	DeleteObject(hwndDesktop);
	DeleteObject(desktopDC);

	while (bRun)
	{
		ProcessMessages();	
	};
}

void WinApp::ProcessMessages(void)
{
	MSG msg;
	while (PeekMessage(&msg, WindowHandle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK WinApp::WinProc(HWND win, UINT msg, WPARAM wp, LPARAM lp)
{

	switch (msg)
	{

	case WM_LBUTTONDOWN:
	{
		CroppingRect::BeginDraw(LOWORD(lp), HIWORD(lp));
		break;
	}

	case WM_LBUTTONUP:
	{
		CroppingRect::EndDraw(LOWORD(lp), HIWORD(lp));

		HWND hwndDesktop = GetDesktopWindow();
		cv::Mat ScreenImage = Convert::hwnd2mat(hwndDesktop);
		cv::Mat croppedImage = CroppingRect::Crop(ScreenImage);
		auto ConvertedText = OCR::Convert(croppedImage);
		std::replace(ConvertedText.begin(), ConvertedText.end(), '\n', ' ');


		ClipboardManager::Paste(ConvertedText);

		DeleteObject(hwndDesktop);
		SendMessage(WindowHandle, WM_DESTROY, wp, lp);
		bRun = false;
		break;
	}

	case WM_MOUSEMOVE:
	{
		if (CroppingRect::IsDrawing())
		{
			CroppingRect::EndPoint.x = LOWORD(lp);
			CroppingRect::EndPoint.y = HIWORD(lp);
		}
	
	}

	case WM_PAINT:
	{
		std::string s = "C:\\temp\\Image.bmp";
		std::wstring stemp = std::wstring(s.begin(), s.end());
		LPCWSTR sw = stemp.c_str();

		HBITMAP hbmObraz = (HBITMAP)LoadImage(NULL, sw, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		HDC hdcNew = CreateCompatibleDC(GetDC(WindowHandle));
		SelectObject(GetDC(WindowHandle), hbmObraz);
		SelectObject(hdcNew, hbmObraz);
		HDC hdcWin = GetDC(WindowHandle);
		BitBlt(hdcWin, 0, 0, fullscreenWidth, fullscreenHeight, hdcNew, 0, 0, SRCCOPY);

		PAINTSTRUCT ps;
		hdcNew = CreateCompatibleDC(GetDC(WindowHandle));
		HDC hdc;

		if (CroppingRect::IsDrawing())
		{
			hdc = BeginPaint(WindowHandle, &ps);
			BitBlt(hdc, 0, 0, fullscreenWidth, fullscreenHeight, hdcNew, 0, 0, SRCCOPY);
			EndPaint(WindowHandle, &ps);
			
			CroppingRect::Draw(GetDC(WindowHandle));
		}
		else
		{
			hdc = BeginPaint(WindowHandle, &ps);
			BitBlt(GetDC(WindowHandle), 0, 0, fullscreenWidth, fullscreenHeight, GetDC(0), 0, 0, SRCCOPY);
			EndPaint(WindowHandle, &ps);
		}

		DeleteObject(hdc);
		DeleteObject(hdcWin);
		DeleteObject(hdcNew);
		DeleteObject(hbmObraz);
		break;
	}

	case WM_CLOSE:
	{
		DestroyWindow(WindowHandle);
		break;
	}
	

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}

	default:
		return DefWindowProc(win, msg, wp, lp);
	}
	
	return 0;
}

