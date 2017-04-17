#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
using namespace cv;
using namespace std;
#define UNDEFINED 0
/*
static char file2open[1024];
int dump(char* fileName, char* buf, long long size)
{
	FILE* file = NULL;
	sprintf_s(file2open, 1024, "%s%s", "./", fileName);
	fopen_s(&file, file2open, "wb");
	if (NULL == file)
	{
		return -1;
	}
	fwrite(buf, 1, (size_t)size, file);
	fclose(file);
	return 0;
}

#define UpCutLen    1
#define DownCutLen  1
#define LeftCutLen  1
#define RightCutLen 1


static int cutImage(uchar *inImage, uchar *outImage, int srcwidth,int srcheight, int& width, int& height)
{


	unsigned int srcy, srcx, dsty, dstx;
	unsigned int dstWidth, dstHeight;
	unsigned int srcWidth, srcHeight;

	srcWidth = srcwidth;
	srcHeight = srcheight;

	dstWidth = srcWidth - LeftCutLen - RightCutLen;
	dstHeight = srcHeight - UpCutLen - DownCutLen;

	width = dstWidth;
	height = dstHeight;

	//outImage.cols = dstWidth;
	//outImage.rows = dstHeight;
	//outImage.data = (unsigned char *)calloc(1, sizeof(unsigned char) * dstWidth * dstHeight * inImage.channels);

	for (srcy = UpCutLen, dsty = 0; srcy < srcHeight - DownCutLen; srcy++, dsty++)
	{
		for (srcx = LeftCutLen, dstx = 0; srcx < srcWidth - RightCutLen; srcx++, dstx++)
		{
			outImage[(dsty * dstWidth + dstx) * 3 + 0] = inImage[(srcy * srcWidth + srcx) * 3 + 0];
			outImage[(dsty * dstWidth + dstx) * 3 + 1] = inImage[(srcy * srcWidth + srcx) * 3 + 1];
			outImage[(dsty * dstWidth + dstx) * 3 + 2] = inImage[(srcy * srcWidth + srcx) * 3 + 2];
		}
	}

	return 0;
}
*/
int main(int argc, char** argv)
{
	VideoCapture cap("image_02.mp4"); //capture the video from web cam

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	int iLowH = 35;
	int iHighH = 77;

	int iLowS = 43;
	int iHighS = 255;

	int iLowV = 46;
	int iHighV = 255;

	//Create trackbars in "Control" window
	cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control", &iHighH, 179);

	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &iHighS, 255);

	cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &iHighV, 255);
	Mat imgThresholded;
	Mat imgHSV;
	while (true)
	{
		Mat imgOriginal;

		bool bSuccess = cap.read(imgOriginal); // read a new frame from video

		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
		
		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

		Mat dst(imgOriginal.rows, imgOriginal.cols, CV_8UC3);

		for (int i = 0; i < imgThresholded.rows; i++)
		{
			for (int j = 0; j < imgThresholded.cols; j++)
			{
				if (imgThresholded.data[i * imgThresholded.cols + j ] == 0)
				{
					dst.data[(i * imgThresholded.cols + j) * 3 + 0] = imgOriginal.data[(i * imgThresholded.cols + j) * 3 + 0];
					dst.data[(i * imgThresholded.cols + j) * 3 + 1] = imgOriginal.data[(i * imgThresholded.cols + j) * 3 + 1];
					dst.data[(i * imgThresholded.cols + j) * 3 + 2] = imgOriginal.data[(i * imgThresholded.cols + j) * 3 + 2];
				
				}
			}
		}

		int width, height;
		//dump("tmp.txt", (char*)imgdst, sizeof(uchar) * imgThresholded.cols * imgThresholded.rows * 3);

		dilate(dst, dst, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
	
		erode(dst, dst, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));

		imshow("Thresholded Image", dst); //show the thresholded image
		imshow("Original", imgOriginal); //show the original image

		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}

	return 0;

}