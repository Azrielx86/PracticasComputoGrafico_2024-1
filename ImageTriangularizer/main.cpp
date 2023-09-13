#include <filesystem>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

int main(int argc, char **argv)
{
	if (argc <= 1)
	{
		std::cerr << "Needed an image.\n";
		return 1;
	}

	std::filesystem::path imgPath(argv[1]);
	std::string winName = imgPath.filename();

	if (!exists(imgPath))
	{
		std::cerr << "File does not exists.\n";
		return 1;
	}

	cv::Mat img, gsImg, dest;
	img = cv::imread(imgPath, cv::IMREAD_ANYCOLOR);

	if (img.empty())
	{
		std::cerr << "File is not an image.\n";
		return 1;
	}

	cv::cvtColor(img, gsImg, cv::COLOR_BGRA2GRAY);

	std::vector<cv::KeyPoint> keypoints;
	// Obtener puntos de interés
	//	cv::Ptr<cv::FeatureDetector> detector = cv::ORB::create(10000, 1, 5);
	//	detector->detect(gsImg, keypoints);
	cv::Mat thresholded;
	//	Filtro tip dibujo jsjs
	cv::adaptiveThreshold(gsImg, thresholded, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 13, 5);

	for (auto const &point : keypoints)
	{
		cv::circle(img, point.pt, 0.0f, cv::Scalar(60, 20, 220), 20.0f);
	}

	cv::namedWindow(winName, 0);
	cv::imshow(winName, thresholded);
	cv::resizeWindow(winName, 1280, 720);

	while (cv::getWindowProperty(winName, cv::WND_PROP_VISIBLE) > 0)
		if (cv::waitKeyEx(1) == 27)
			break;

	cv::destroyAllWindows();

	return 0;
}
