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
	double x, y, z;                                               //����������
	double length();                                              //��������
	friend vector3 operator - (const vector3& a, const vector3& b);  //��������
	friend vector3 operator + (const vector3& a, const vector3& b);  //�����ӷ�
	friend double operator * (const vector3& a, const vector3& b);   //�������
	friend vector3 operator * (double f, const vector3& a);          //�������������
	friend vector3 operator * (const vector3& a, double f);          //�����볣�����
	void unitization();
	void show();
	double get(int dim);
};

#endif