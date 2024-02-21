#pragma once
#include <Windows.h>
#include <string>

class BMP
{
	HBITMAP BMapHandle;
	BITMAP bmInfo;
	HDC BMP_HDC;
	std::wstring FileLocalization;

public:
	BMP(std::wstring FileLocalization);
	~BMP();
	void Draw();

private:
	void Load(std::wstring FileLocalization);
	void Save();
};
