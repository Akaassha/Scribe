#pragma once
#include <Windows.h>
#include <string>

class ClipboardManager
{
public:
	static void Init(HWND WindowHandle);
	static void Paste(std::string data);
};

