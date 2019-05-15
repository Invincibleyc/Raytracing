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
	vector3 origin;  //���ߵ����
	vector3 dir;     //���ߵķ���
	Ray(vector3 _origin, vector3 _dir): origin(_origin){ dir=_dir; dir.unitization();}
};

class Light{
public:
	vector3 center;  //��Դ������
	vector3 color;   //��Դ����ɫ
	double radius;   //Բ�����Դ�뾶
	Light(){center = vector3(0.0, 0.0, 0.0), color = vector3(0.0, 0.0, 0.0), radius = 0.0;}
	Light(vector3 _center, vector3 _color, double _radius): center(_center), color(_color), radius(_radius){}
};

class point{
public:
	int x;
	int y;
};
#endif