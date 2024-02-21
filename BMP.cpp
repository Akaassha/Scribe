#include "BMP.h"
#include "WinApp.h"

//std::string s = "c:\\Cpp\\plik.bmp";
//std::wstring stemp = std::wstring(s.begin(), s.end());
//LPCWSTR sw = stemp.c_str();
//hbmObraz = (HBITMAP)LoadImage(NULL, sw, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//HDC hdcNowy = CreateCompatibleDC(GetDC(WindowHandle));
//
//SelectObject(hdcNowy, hbmObraz);
//
//BITMAP bmInfo;
//GetObject(hbmObraz, sizeof(bmInfo), &bmInfo);
//BitBlt(GetDC(WindowHandle), 50, 50, bmInfo.bmWidth, bmInfo.bmHeight, hdcNowy, 0, 0, SRCCOPY);
//ReleaseDC(WindowHandle, GetDC(WindowHandle));

BMP::BMP(std::wstring FileLocalization) : FileLocalization{ FileLocalization }
{
	Load(FileLocalization);
	BMP_HDC = CreateCompatibleDC(GetDC(WinApp::GetHandle()));
	SelectObject(BMP_HDC, BMapHandle);
	GetObject(BMapHandle, sizeof(bmInfo), &bmInfo);
}

void BMP::Draw()
{
	BitBlt(GetDC(WinApp::GetHandle()), 50, 50, bmInfo.bmWidth, bmInfo.bmHeight, BMP_HDC, 0, 0, SRCCOPY);
}


void BMP::Load(std::wstring FileLocalization)
{
	LPCWSTR sw = FileLocalization.c_str();
	BMapHandle = (HBITMAP)LoadImage(NULL, sw, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

void BMP::Save()
{

}

BMP::~BMP()
{
	ReleaseDC(WinApp::GetHandle(), BMP_HDC);
}