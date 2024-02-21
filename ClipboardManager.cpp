#include "ClipboardManager.h"

void ClipboardManager::Init(HWND WindowHandle)
{
	OpenClipboard(WindowHandle);
}

void ClipboardManager::Paste(std::string data)
{
	
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, data.size() + 1);
	if (!hg) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), data.c_str(), data.size() + 1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}
