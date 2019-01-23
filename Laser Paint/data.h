#pragma once

#include <opencv2\core.hpp>
#include <string>

namespace data
{
	namespace color
	{
		namespace HSV
		{
			int H_MIN = 0;
			int H_MAX = 10;
			int S_MIN = 0;
			int S_MAX = 255;
			int V_MIN = 200;
			int V_MAX = 255;
		}

		namespace RGB
		{
			int R = 255;
			int G = 255;
			int B = 255;
		}

		cv::Scalar minHSV(HSV::H_MIN, HSV::S_MIN, HSV::V_MIN);
		cv::Scalar maxHSV(HSV::H_MAX, HSV::S_MAX, HSV::V_MAX);

		cv::Scalar pictureColor(0, 0, 255);

		int value = 200;
	}

	namespace window
	{
		std::string name = "board";
		std::string title = "Laser Paint";
	}

	namespace camera
	{
		int width = 1280;
		int height = 720;
		int cameraPointer = 1;

		// 0 - Laptop kamera
		// 1 - Additional webcam
	}

	namespace board
	{
		int border = 5;
		int thinkness = 10;

		cv::Scalar color(color::RGB::B, color::RGB::G, color::RGB::R);
	}

	std::vector<int> compression_params;
}

/*

	About "Laser Paint"

	esc - exit
	s - make a screenshot
	c - clean a board

*/
