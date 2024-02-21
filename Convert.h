#pragma once
#include <Windows.h>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

class Convert
{
public:

	static HBITMAP hwnd2bitmap(HWND windowHandle);
	static cv::Mat hwnd2mat(HWND hwnd);
	static cv::Mat bitmap2mat(HBITMAP hwnd);
};

