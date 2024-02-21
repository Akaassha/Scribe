#include "CroppingRect.h"


bool CroppingRect::bIsDrawing = false;
POINT CroppingRect::StartPoint = POINT{};
POINT CroppingRect::EndPoint = POINT{};

unsigned int CroppingRect::padding = 2;

void CroppingRect::Draw(HDC hdc)
{
	HPEN redPen = CreatePen(PS_SOLID, 2, 0x0000FF);
	HPEN oldPen = (HPEN)SelectObject(hdc, redPen);

	POINT oldCoords;

	MoveToEx(hdc, StartPoint.x, StartPoint.y, &oldCoords);
	LineTo(hdc, StartPoint.x, EndPoint.y);
	LineTo(hdc, EndPoint.x, EndPoint.y);
	LineTo(hdc, EndPoint.x, StartPoint.y);
	LineTo(hdc, StartPoint.x, StartPoint.y);

	DeleteObject(redPen);
	DeleteObject(oldPen);
}

cv::Mat CroppingRect::Crop(cv::Mat sourceImage)
{
	unsigned int leftup_x = (StartPoint.x >= EndPoint.x) ? EndPoint.x : StartPoint.x;
	unsigned int leftup_y = (StartPoint.y >= EndPoint.y) ? EndPoint.y : StartPoint.y;
	unsigned int width  = abs(EndPoint.x - StartPoint.x);
	unsigned int height = abs(EndPoint.y - StartPoint.y);

	cv::Rect myROI(leftup_x + padding,
		leftup_y + padding,
		(width)  + padding,
		(height) + padding);


	cv::Mat croppedImage = sourceImage(myROI);

	return croppedImage;
}

void CroppingRect::BeginDraw(unsigned int x, unsigned int y)
{
	StartPoint.x = x;
	StartPoint.y = y;
	bIsDrawing = true;
}

void CroppingRect::EndDraw(unsigned int x, unsigned int y)
{
	EndPoint.x = x;
	EndPoint.y = y;
	bIsDrawing = false;
}

bool CroppingRect::IsDrawing()
{
	return bIsDrawing;
}