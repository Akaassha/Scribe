#include "OCR.h"

std::string OCR::tessdataPath = "C:\temp\tessdata";
std::string OCR::language = "eng";
std::string OCR::outText = "";

void OCR::Init(std::string tessdataPath, std::string language)
{
	OCR::tessdataPath = tessdataPath;
	OCR::language = language;
}

std::string OCR::Convert(std::string path)
{
	tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
	if (api->Init(tessdataPath.c_str(), language.c_str()))
	{
		exit(1);
	}

	Pix* image = pixRead(path.c_str());
	api->SetImage(image);

	outText = api->GetUTF8Text();
	return outText;
}

 std::string OCR::Convert(cv::Mat image)
{
	 tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
	 if (api->Init(tessdataPath.c_str(), language.c_str()))
	 {
		 exit(1);
	 }

	api->SetPageSegMode(tesseract::PSM_AUTO);
	api->SetImage(image.data, image.cols, image.rows, 4, image.step);
	return std::string(api->GetUTF8Text());

}

