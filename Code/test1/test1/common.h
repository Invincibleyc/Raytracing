#ifndef COMMON_H
#define COMMON_H

#include "vector3.h"
#include <fstream>
#include <ctime>
#include "opencv2\opencv.hpp"
using namespace cv;

const int scale = 3;
const double pi = 3.1415926535897;
const int lightnumber = 16;

class Ray{
public:
	vector3 origin;  //光线的起点
	vector3 dir;     //光线的方向
	Ray(vector3 _origin, vector3 _dir): origin(_origin){ dir=_dir; dir.unitization();}
};

class Light{
public:
	vector3 center;  //光源的中心
	vector3 color;   //光源的颜色
	double radius;   //圆形面光源半径
	Light(){center = vector3(0.0, 0.0, 0.0), color = vector3(0.0, 0.0, 0.0), radius = 0.0;}
	Light(vector3 _center, vector3 _color, double _radius): center(_center), color(_color), radius(_radius){}
};

class point{
public:
	int x;
	int y;
};
#endif