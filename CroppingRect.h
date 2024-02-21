#pragma once
#include <Windows.h>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

class CroppingRect
{
public:
	static POINT StartPoint;
	static POINT EndPoint;
	
	static void BeginDraw(unsigned int x, unsigned int y);
	static void EndDraw(unsigned int x, unsigned int y);
	static void Draw(HDC hdc);
	static cv::Mat Crop(cv::Mat sourceImage);

	static bool IsDrawing();

private:
	static bool bIsDrawing;
	static unsigned int padding;
};

