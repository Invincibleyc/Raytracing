#ifndef VECTOR3_H
#define VECTOR3_H

#include<cmath>
#include <iostream>
using namespace std;

class vector3
{
public:
	vector3();
	vector3(double _x, double _y, double _z);
	vector3(const vector3& v);
	~vector3();
	double x, y, z;                                               //横纵竖坐标
	double length();                                              //向量长度
	friend vector3 operator - (const vector3& a, const vector3& b);  //向量减法
	friend vector3 operator + (const vector3& a, const vector3& b);  //向量加法
	friend double operator * (const vector3& a, const vector3& b);   //向量点乘
	friend vector3 operator * (double f, const vector3& a);          //常数与向量相乘
	friend vector3 operator * (const vector3& a, double f);          //向量与常数相乘
	void unitization();
	void show();
	double get(int dim);
};

#endif