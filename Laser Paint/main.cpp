#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <math.h>

#include "data.h"

#if not _DEBUG

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#endif

using namespace cv;
using namespace data;

struct drawPoint
{
	Point2d position;
	Scalar color;

	int thinkness;
};

static int variable = 1;

void getPointFromMat(Mat &board, std::vector<drawPoint> &points)
{
	Point firstPos = { 0 }, lastPos = { 0 };
	Point2d center;

	for (int y = board::border; y < board.rows - board::border; y++)
	{
		for (int x = board::border; x < board.cols - board::border; x++)
		{
			Vec3b colour = board.at<Vec3b>(Point(x, y));

			if (colour[0] >= color::value && colour[1] >= color::value && colour[2] >= color::value)
			{
				if (firstPos == Point(0, 0))
				{
					firstPos = Point(x, y);
				}
				else
				{
					lastPos = Point(x, y);
				}
			}
		}
	}

	if (firstPos.x <= lastPos.x)
	{
		center.x = lastPos.x - abs(firstPos.x - lastPos.x);
	}
	else if (firstPos.x > lastPos.x)
	{
		center.x = firstPos.x - abs(firstPos.x - lastPos.x);
	}

	if (firstPos.y <= lastPos.y)
	{
		center.y = lastPos.y - abs(firstPos.y - lastPos.y);
	}
	else if (firstPos.y > lastPos.y)
	{
		center.y = firstPos.y - abs(firstPos.y - lastPos.y);
	}

	if (center != Point2d(0, 0))
	{
		drawPoint point;

		point.color = board::color;
		point.thinkness = board::thinkness;
		point.position = center;

		points.push_back(point);
	}
}

void draw(Mat &board, std::vector<drawPoint> points)
{
	rectangle(board, Rect(0, 0, camera::width, camera::height), Scalar(0, 0, 0), -1);

	for (int i = 0; i < points.size(); i++)
	{
		circle(board, points[i].position, points[i].thinkness / 2, points[i].color, -1);

		float distance = 0;

		if (i < points.size() - 1)
			distance = float(sqrt(pow(abs(points[i].position.x - points[i + 1].position.x), 2) + pow(abs(points[i].position.y - points[i + 1].position.y), 2)));

		if (distance > 10 && distance < 200)
		{
			line(board, points[i].position, points[i + 1].position, points[i].color, points[i].thinkness);
		}
	}
}

void setupControlPanel(String name, String title)
{
	namedWindow(name, WINDOW_AUTOSIZE);
	setWindowTitle(name, title);

	createTrackbar("R", name, &color::RGB::R, 255);
	createTrackbar("G", name, &color::RGB::G, 255);
	createTrackbar("B", name, &color::RGB::B, 255);
	createTrackbar("T", name, &board::thinkness, 50);
}

void changeValues(String name)
{
	color::RGB::R = getTrackbarPos("R", name);
	color::RGB::G = getTrackbarPos("G", name);
	color::RGB::B = getTrackbarPos("B", name);

	board::thinkness = getTrackbarPos("T", name);

	board::color = Scalar(color::RGB::B, color::RGB::G, color::RGB::R);
}

inline void setupCamera(VideoCapture * camera)
{
	camera->open(camera::cameraPointer);

	if (camera->isOpened())
	{
		camera->set(CAP_PROP_FRAME_WIDTH, camera::width);
		camera->set(CAP_PROP_FRAME_HEIGHT, camera::height);
		camera->set(CAP_PROP_BUFFERSIZE, 3);
	}

	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);
}

int main(int argc, char ** argv)
{
	VideoCapture camera;

	setupCamera(&camera);

	if (!camera.isOpened())
	{
		std::cerr << "Can not open web cam!" << std::endl;

		return -1;
	}

	namedWindow(String(window::name), WINDOW_NORMAL);
	setWindowProperty(String(window::name), WND_PROP_FULLSCREEN, WINDOW_AUTOSIZE);
	setWindowTitle(String(window::name), String(window::title));

	setupControlPanel("panel", "Control Panel");

	std::vector<drawPoint> points;

	while (waitKey(30) != 27)
	{
		Mat board;

		try
		{
			camera.read(board);
		}
		catch (std::string e)
		{
			std::cerr << e << std::endl;
			std::cerr << "Can not read image from camera!" << std::endl;

			return -1;
		}

		getPointFromMat(board, points);

		draw(board, points);

		imshow(String(window::name), board);

		if (waitKey(30) == 's')
		{
			std::string fileName = "Screenshot_" + std::to_string(variable) + ".png";

			try
			{
				imwrite(String(fileName), board, compression_params);
			}
			catch (std::string e)
			{
				std::cerr << e << std::endl;
				std::cerr << "Can not save a screenshot!" << std::endl;
			}

			points.clear();
			
			variable++;
		}
		else if (waitKey(30) == 'c')
		{
			points.clear();
		}

		changeValues("panel");
	}

	destroyAllWindows();

	return 0;
}