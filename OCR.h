#pragma once
#include <string>
#include <baseapi.h>
#include <allheaders.h>

#include "opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

class OCR
{
	static std::string tessdataPath;
	static std::string language;
	static std::string outText;
public:


	static void Init(std::string tessdataPath, std::string language);
	static std::string Convert(std::string path);
	static std::string Convert(cv::Mat image);
};

