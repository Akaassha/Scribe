#include "Files.h"
#include <vector>    // HDCToFile
#include <fstream>
#include <cstring>


bool FileManager::HDCToFile(const char* FilePath, HDC Context, unsigned int BitsPerPixel)
{
    BITMAP structBitmapHeader;
    memset(&structBitmapHeader, 0, sizeof(BITMAP));

    HGDIOBJ hBitmap = GetCurrentObject(Context, OBJ_BITMAP);
    GetObject(hBitmap, sizeof(BITMAP), &structBitmapHeader);

    uint32_t Width = structBitmapHeader.bmWidth;
    uint32_t Height = structBitmapHeader.bmHeight;

    BITMAPINFO Info;
    BITMAPFILEHEADER Header;
    memset(&Info, 0, sizeof(Info));
    memset(&Header, 0, sizeof(Header));
    Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

    Info.bmiHeader.biWidth = Width;
    Info.bmiHeader.biHeight = Height;

    Info.bmiHeader.biPlanes = 1;
    Info.bmiHeader.biBitCount = BitsPerPixel;
    Info.bmiHeader.biCompression = BI_RGB;
    Info.bmiHeader.biSizeImage = Width * Height * (BitsPerPixel > 24 ? 4 : 3);

    Header.bfType = 0x4D42;
    Header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    //uint32_t Width = Area.right - Area.left;
   // uint32_t Height = Area.bottom - Area.top;

    memset(&Info, 0, sizeof(Info));
    memset(&Header, 0, sizeof(Header));
    Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    Info.bmiHeader.biWidth = Width;
    Info.bmiHeader.biHeight = Height;
    Info.bmiHeader.biPlanes = 1;
    Info.bmiHeader.biBitCount = BitsPerPixel;
    Info.bmiHeader.biCompression = BI_RGB;
    Info.bmiHeader.biSizeImage = Width * Height * (BitsPerPixel > 24 ? 4 : 3);
    Header.bfType = 0x4D42;
    Header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


    char* Pixels = NULL;
    HDC MemDC = CreateCompatibleDC(Context);
    HBITMAP Section = CreateDIBSection(Context, &Info, DIB_RGB_COLORS, (void**)&Pixels, 0, 0);
    DeleteObject(SelectObject(MemDC, Section));
    BitBlt(MemDC, 0, 0, Info.bmiHeader.biWidth, Info.bmiHeader.biHeight, Context, 0, 0, SRCCOPY);
    DeleteDC(MemDC);

    std::fstream hFile(FilePath, std::ios::out | std::ios::binary);
    if (hFile.is_open())
    {
        hFile.write((char*)&Header, sizeof(Header));
        hFile.write((char*)&Info.bmiHeader, sizeof(Info.bmiHeader));
        hFile.write(Pixels, (((BitsPerPixel * Info.bmiHeader.biWidth + 31) & ~31) / 8) * Info.bmiHeader.biHeight);
        hFile.close();
        DeleteObject(Section);
        return true;
    }

    DeleteObject(Section);
    return false;
}