#include "common.h"
#include "Raytracer.h"
#include<opencv2\opencv.hpp>
using namespace cv;

void showvector(vector3 a);

int main(){
	/*Mat wall = imread("pattern6.jpg");
	Mat _img = Mat::zeros(300, 300, CV_8UC3);
	resize(wall, _img, _img.size(), CV_INTER_AREA);
	imwrite("pattern7.png", _img);*/
	/*vector3 a(2.0f, 3.0f, 4.0f);
	vector3 b(5.0f, 1.0f, 6.0f);
	unitization(a);
	showvector(3.0f*a);*/
	/*Mat wall = imread("wall4.jpg");
	Mat _img = Mat::zeros(500, 460, CV_8UC3);
	resize(wall, _img, _img.size(), CV_INTER_AREA);
	imwrite("wall4.bmp", _img);
	namedWindow("dst", CV_WINDOW_AUTOSIZE );
	imshow("dst", wall);
	waitKey(0);*/

	Raytracer tracer;
	Mat img = tracer.draw();
	Mat kernel = (Mat_<double>(3,3) << 1.0/16.0, 2.0/16.0, 1.0/16.0,
								   2.0/16.0, 4.0/16.0, 2.0/16.0,
								   1.0/16.0, 2.0/16.0, 1.0/16.0);
	Mat _img = Mat::zeros(800, 800, CV_8UC3);
	resize(img, _img, _img.size(), CV_INTER_AREA);
	Mat dst;
	filter2D(_img, dst, _img.depth(),kernel);
	vector<int> compression_params;
    //compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION); //PNG格式图片的压缩级别  
    //compression_params.push_back(9);  //这里设置保存的图像质量级别
	imwrite("dst38-6.bmp", dst);
	namedWindow("dst", CV_WINDOW_AUTOSIZE);
	imshow("dst", dst);
	waitKey(0); 
	destroyWindow("dst");

	return 0;
}

void showvector(vector3 a){
	cout << a.x << " " << a.y << " " << a.z << endl;
}