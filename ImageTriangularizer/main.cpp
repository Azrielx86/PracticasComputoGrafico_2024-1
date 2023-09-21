#include <algorithm>
#include <filesystem>
#include <iostream>
#include <map>
#include <opencv2/core.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

typedef cv::Point3_<uint8_t> Pixel;

// https://stackoverflow.com/a/34734939/5008845
void reduceColor_kmeans(const cv::Mat &src, cv::Mat &dst)
{
	int K = 8;
	int n = src.rows * src.cols;
	cv::Mat data = src.reshape(1, n);
	data.convertTo(data, CV_32F);

	std::vector<int> labels;
	cv::Mat1f colors;
	cv::kmeans(data, K, labels, cv::TermCriteria(), 1, cv::KMEANS_PP_CENTERS, colors);

	int i;
#pragma omp parallel for private(i)
	for (i = 0; i < n; ++i)
	{
		data.at<float>(i, 0) = colors(labels[i], 0);
		data.at<float>(i, 1) = colors(labels[i], 1);
		data.at<float>(i, 2) = colors(labels[i], 2);
	}

	cv::Mat reduced = data.reshape(3, src.rows);
	reduced.convertTo(dst, CV_8U);
}

void show_image(const std::string &winName, const cv::Mat &img)
{
	cv::namedWindow(winName, 0);
	cv::imshow(winName, img);
	cv::resizeWindow(winName, 1280, 720);

	while (cv::getWindowProperty(winName, cv::WND_PROP_VISIBLE) > 0)
		if (cv::waitKeyEx(1) == 27)
			break;

	if (cv::getWindowProperty(winName, cv::WND_PROP_VISIBLE) != 0)
		cv::destroyWindow(winName);
}

std::map<int, Pixel> separate_color(const cv::Mat &img)
{
	std::map<int, Pixel> imgColors;
	img.forEach<Pixel>([&imgColors](Pixel &p, const int *pos) -> void
	                   { imgColors[(p.x + p.y + p.z) << 5] = p; });
	return imgColors;
}

cv::Mat get_mask_img(cv::Mat &img, Pixel c)
{
	cv::Mat mask, output;
	img.copyTo(output);

	int i, j;
#pragma omp parallel for private(i, j)
	for (i = 0; i < img.rows; i++)
	{
		for (j = 0; j < img.cols; j++)
		{
			auto p = img.at<Pixel>(i, j);
			if (!(p.x == c.x && p.y == c.y && p.z == c.z))
			{
				output.at<Pixel>(i, j) = Pixel(0, 0, 0);
			}
		}
	}

	//	cv::Mat mask, output_range;
	//	auto color = cv::Scalar(c.z, c.y, c.x);
	//	cv::inRange(img, color, color, mask);
	//	cv::bitwise_and(img, img, output_range, mask);

	//	mask = cv::Mat(img.rows, img.cols, CV_8UC3, cv::Scalar(color.z, color.y, color.x));
	//	cv::bitwise_and(img, mask, output_range);
	return output;
}

int main(int argc, char **argv)
{
	if (argc <= 1)
	{
		std::cerr << "Needed an image.\n";
		return 1;
	}

	std::filesystem::path imgPath(argv[1]);
	std::string winName = imgPath.filename().string();

	if (!exists(imgPath))
	{
		std::cerr << "File does not exists.\n";
		return 1;
	}

	cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);
	cv::Mat img, reduced;
	img = cv::imread(imgPath.string(), -1);

	if (img.empty())
	{
		std::cerr << "File is not an image.\n";
		return 1;
	}

#ifdef PRUEBAS
	cv::cvtColor(img, gsImg, cv::COLOR_BGRA2GRAY);

	std::vector<cv::KeyPoint> keypoints;
	// Obtener puntos de interés
	//	cv::Ptr<cv::FeatureDetector> detector = cv::ORB::create(10000, 1, 5);
	//	detector->detect(gsImg, keypoints);
	cv::Mat thresholded;
	//	Filtro tip dibujo jsjs
	cv::adaptiveThreshold(gsImg, thresholded, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 11, 5);
	//	cv::findContours(gsImg)

	//	cv::approxPolyDP()

	for (auto const &point : keypoints)
	{
		cv::circle(img, point.pt, 0.0f, cv::Scalar(60, 20, 220), 20.0f);
	}
#endif

	reduceColor_kmeans(img, reduced);
	show_image(winName, img);
	show_image("Reduced", reduced);

	auto colors = separate_color(reduced);

	std::for_each(colors.begin(), colors.end(), [&reduced](std::pair<int, Pixel> v) -> void
	              { 
		              printf("[%d, %d, %d]\n", v.second.z, v.second.y, v.second.x);
		              auto ft = get_mask_img(reduced, v.second);
		              show_image("Color", ft); });

	//	Pixel pixel(51, 43, 40);
	//	auto result = get_mask_img(img, pixel);
	//	show_image("biu", result);

	std::cout << "End!";

	return 0;
}
