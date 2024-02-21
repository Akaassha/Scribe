#include <Windows.h>
#include "WinApp.h"
#include "OCR.h"
#include "ClipboardManager.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	WinApp app(instance);
	OCR::Init("C:\\Program Files\\Tesseract-OCR\\tessdata", "eng");
	ClipboardManager::Init(app.GetHandle());

	app.Run();

	return 0;
}
