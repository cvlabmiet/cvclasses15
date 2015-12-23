#include <algorithm>

#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <opencv2\calib3d\calib3d.hpp>

#include <iostream>
#include <vector>
#include <algorithm>

using namespace cv;
using namespace std;

void opencv_stereo_matching(Mat& left, Mat& right, Mat& output);
void camera1(int number_camera, Mat& camera_mat, Mat& dist_coeffs, const int count_frames);
void camera2(Mat& camera_mat0, Mat& dist_coeffs0, Mat& camera_mat1, Mat& dist_coeffs1, const int count_frames);
void calibrate(Mat& map0x, Mat& map0y, Mat& map1x, Mat& map1y);

int main()
{
	Mat map0x;
	Mat map0y;
	Mat map1x;
	Mat map1y;
	calibrate(map0x, map0y, map1x, map1y);

	VideoCapture video0 = VideoCapture(0);
	VideoCapture video1 = VideoCapture(1);
	Mat frame0;
	Mat frame1;
	while (true)
	{
		video0 >> frame0;
		video1 >> frame1;
		Mat newframe0, newframe1;
		remap(frame0, newframe0, map0x, map0y, INTER_LINEAR, BORDER_CONSTANT, Scalar());
		remap(frame1, newframe1, map1x, map1y, INTER_LINEAR, BORDER_CONSTANT, Scalar());
		cv::Mat q0 = cv::Mat::zeros(newframe0.rows, newframe0.cols, CV_8U);
		opencv_stereo_matching(newframe0, newframe1, q0);
		imshow("Undistort0", q0);
		//imshow("Undistort1", frame1);
		//system("pause");
		char c = cvWaitKey(30);
		if (c == 27)
		{
			break;
		}
	}

	video0.release();
	video1.release();

	return 0;
}

void camera1(int number_camera, Mat& camera_mat, Mat& dist_coeffs, const int count_frames)
{
	VideoCapture video = VideoCapture(number_camera);
	namedWindow("Original");

	Mat frame;
	for (int i = 0; i < 50; i++)
	{
		video >> frame;
		if (frame.empty())
		{
			std::cout << "Video error camera # " << number_camera << "\n";
			continue;
		}
		imshow("Original", frame);
		cvWaitKey(30);
	}

	Mat left;
	Size patternsize(9, 7);
	vector<vector<Point2f>> corners(count_frames);

	int pattern_counter = 0;

	while (true)
	{
		video >> frame;
		Mat gray;
		cvtColor(frame, gray, CV_BGR2GRAY);
		bool patternfound;
		patternfound = findChessboardCorners(gray, patternsize,
			corners[pattern_counter], CALIB_CB_ADAPTIVE_THRESH
			+ CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK);
		if (patternfound)
		{
			cornerSubPix(gray, corners[pattern_counter], Size(11, 11), Size(-1, -1),
				TermCriteria(TermCriteria::COUNT + TermCriteria::EPS,
				30, 0.01));
		}
		drawChessboardCorners(gray, patternsize, Mat(corners[pattern_counter]),
			patternfound);
		imshow("Original", gray);

		if (patternfound)
		{
			char c = cvWaitKey(0);
			if (c == 32)
			{
				pattern_counter++;
				std::cout << pattern_counter << " \n";
			}
			if (pattern_counter >= corners.size())
			{
				break;
			}
		}

		char c = cvWaitKey(30);
		if (c == 27)
		{
			break;
		}
	}

	destroyAllWindows();
	std::cout << "Start Calibration\n";
	float square_size = 1.;
	vector<vector<Point3f>> object(pattern_counter);
	for (int k = 0; k < pattern_counter; k++)
	{
		for (int y = 0; y < patternsize.height; y++)
		{
			for (int x = 0; x < patternsize.width; x++)
			{
				object[k].push_back(Point3f(x*square_size, y*square_size, 0));
			}
		}
	}
	vector<Mat> tcrit, rcrit;
	calibrateCamera(object, corners, frame.size(), camera_mat, dist_coeffs,
		rcrit, tcrit,
		CV_CALIB_FIX_ASPECT_RATIO +
		CV_CALIB_ZERO_TANGENT_DIST +
		CV_CALIB_RATIONAL_MODEL +
		CV_CALIB_FIX_K3 + CV_CALIB_FIX_K4 + CV_CALIB_FIX_K5,
		TermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 100, 1e-5));

	while (true)
	{
		video >> frame;
		imshow("Original", frame);
		Mat newframe;
		undistort(frame, newframe, camera_mat, dist_coeffs);

		imshow("Undistort", newframe);
		char c = cvWaitKey(30);
		if (c == 27)
		{
			break;
		}
	}
	std::cout << "Done Calibration\n";
	destroyAllWindows();
}

void camera2(Mat camera_mat0, Mat dist_coeffs0, Mat camera_mat1, Mat dist_coeffs1, int count_frames)
{
	VideoCapture video0 = VideoCapture(0);
	namedWindow("Original0");

	VideoCapture video1 = VideoCapture(1);
	namedWindow("Original1");

	Mat frame0;
	Mat frame1;


	// ѕропускаем первые несколько кадров дл€ стабилизации €ркости.
	for (int i = 0; i < 50; i++)
	{
		video0 >> frame0;
		video1 >> frame1;
		if (frame0.empty() || frame1.empty())
		{
			std::cout << "Video error \n";
			continue;
		}
		imshow("Original0", frame0);
		imshow("Original1", frame1);
		cvWaitKey(30);
	}

	Size patternsize(9, 7); // ”станавливаем количество внутренних узлов доски.
	vector<vector<Point2f>> corners0(count_frames); // ћассив дл€ хранени€ найденых шаблонов.
	vector<vector<Point2f>> corners1(count_frames); // ћассив дл€ хранени€ найденых шаблонов.

	int pattern_counter = 0; //  оличество найденый шаблонов

	while (true)
	{
		video0 >> frame0;
		video1 >> frame1;
		Mat gray0;
		Mat gray1;
		cvtColor(frame0, gray0, CV_BGR2GRAY);
		cvtColor(frame1, gray1, CV_BGR2GRAY);
		// »щем углы шахматной доски на изображении
		bool patternfound0;
		bool patternfound1;
		patternfound0 = findChessboardCorners(gray0, patternsize, corners0[pattern_counter],
			CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK);
		patternfound1 = findChessboardCorners(gray1, patternsize, corners1[pattern_counter],
			CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK);
		if (patternfound0)
		{
			// ”точн€ем положени€ углов на изображении.
			cornerSubPix(gray0, corners0[pattern_counter], Size(11, 11), Size(-1, -1),
				TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 30, 0.01));
			// –исуем найденные углы на изображении.
			drawChessboardCorners(gray0, patternsize, Mat(corners0[pattern_counter]), patternfound0);
		}

		if (patternfound1)
		{
			// ”точн€ем положени€ углов на изображении.
			cornerSubPix(gray1, corners1[pattern_counter], Size(11, 11), Size(-1, -1),
				TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 30, 0.01));
			// –исуем найденные углы на изображении.
			drawChessboardCorners(gray1, patternsize, Mat(corners1[pattern_counter]), patternfound1);
		}
		imshow("Original0", gray0);
		imshow("Original1", gray1);

		// ƒл€ добавлени€ шаблона доски нужно нажать пробел,
		// ƒл€ продолжени€ нужно нажать любую другую клавишу.
		if (patternfound0 && patternfound1)
		{
			char c = cvWaitKey(0);
			if (c == 32) //пробел
			{
				pattern_counter++;
				std::cout << pattern_counter << "  \n";
			}
			if (pattern_counter >= corners0.size())
			{
				break;
			}
		}

		char c = cvWaitKey(30);
		if (c == 27) //esc
		{
			break;
		}
	}
	std::cout << "Start Calibration\n";
	destroyAllWindows();
	// ќпредел€ем теоретические координаты точек шаблона.
	float square_size = 1.;
	vector<vector<Point3f>> object(pattern_counter);
	for (int k = 0; k < pattern_counter; k++)
	{
		for (int y = 0; y < patternsize.height; y++)
		{
			for (int x = 0; x < patternsize.width; x++)
			{
				object[k].push_back(Point3f(x * square_size, y * square_size, 0));
			}
		}
	}

	// Ќаходим внутренние параметры камеры.
	Mat R, T, E, F;

	stereoCalibrate(object, corners0, corners1, camera_mat0, dist_coeffs0, camera_mat1, dist_coeffs1, frame0.size(), R, T, E, F,
		CV_CALIB_SAME_FOCAL_LENGTH | CV_CALIB_ZERO_TANGENT_DIST, TermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 100, 1e-5));

	Mat R0, R1, P0, P1, Q;
	stereoRectify(camera_mat0, dist_coeffs0, camera_mat1, dist_coeffs1, frame0.size(), R, T, R0, R1, P0, P1, Q);

	Mat map0x;
	Mat map0y;
	Mat map1x;
	Mat map1y;
	
	initUndistortRectifyMap(camera_mat0, dist_coeffs0, R0, P0, frame0.size(), CV_32FC1, map0x, map0y);
	initUndistortRectifyMap(camera_mat1, dist_coeffs1, R1, P1, frame1.size(), CV_32FC1, map1x, map1y);

	FileStorage fs1("mystereocalib.yml", FileStorage::WRITE);
	fs1 << "map0x" << map0x;
	fs1 << "map0y" << map0y;
	fs1 << "map1x" << map1x;
	fs1 << "map1y" << map1y;
	fs1.release();

	std::cout << "Done Calibration\n";
	video0.release();
	video1.release();
}

void calibrate(Mat& map0x, Mat& map0y, Mat& map1x, Mat& map1y)
{
	cout << "Do you want calibrate camera? (y/n)\n";
	char answer;
	cin >> answer;
	if (answer == 'y')
	{
		Mat camera_mat0 = Mat::eye(3, 3, DataType<double>::type);
		Mat camera_mat1 = Mat::eye(3, 3, DataType<double>::type);
		Mat dist_coeffs0 = Mat::zeros(8, 1, DataType<double>::type);
		Mat dist_coeffs1 = Mat::zeros(8, 1, DataType<double>::type);

		int count_frames = 10;
		camera1(0, camera_mat0, dist_coeffs0, count_frames);
		camera1(1, camera_mat1, dist_coeffs1, count_frames);
		camera2(camera_mat0, camera_mat1, dist_coeffs0, dist_coeffs1, count_frames);
	}

	FileStorage fs;
	fs.open("mystereocalib.yml", FileStorage::READ);

	if (!fs.isOpened())
	{
		cerr << "Failed to open mystereocalib.yml" << endl;
	}
	fs["map0x"] >> map0x;
	fs["map0y"] >> map0y;
	fs["map1x"] >> map1x;
	fs["map1y"] >> map1y;
	fs.release();
}

void opencv_stereo_matching(Mat& left, Mat& right, Mat& output)
{
	cv::Mat gray0;
	cv::Mat gray1;
	cvtColor(left, gray0, CV_BGR2GRAY);
	cvtColor(right, gray1, CV_BGR2GRAY);
	cv::Mat disparity_left = cv::Mat(left.rows, left.cols, CV_16S);// cv::DataType<short>::type);
	Ptr<StereoSGBM> sbm = StereoSGBM::create(0, 16, 3);
	sbm->setDisp12MaxDiff(1);
	sbm->setSpeckleRange(32);
	sbm->setSpeckleWindowSize(10);
	sbm->setUniquenessRatio(0);
	sbm->setMinDisparity(-20);
	int SADWindowSize = 0;
	int sgbmWinSize = 8;
	sbm->setBlockSize(sgbmWinSize);
	sbm->setP1(8 * sgbmWinSize * sgbmWinSize);
	sbm->setP2(32 * sgbmWinSize * sgbmWinSize);
	sbm->setPreFilterCap(70);
	sbm->setMode(StereoSGBM::MODE_HH);
	sbm->compute(gray0, gray1, disparity_left);
	normalize(disparity_left, output, 0, 255, CV_MINMAX, CV_8U);

	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(output, output, MORPH_CLOSE, kernel, Point(-1, -1), 1, BORDER_CONSTANT);
}

