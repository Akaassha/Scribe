#include <windows.h>

class FileManager
{
public:
	static bool HDCToFile(const char* FilePath, HDC Context, unsigned int BitsPerPixel = 24);
};