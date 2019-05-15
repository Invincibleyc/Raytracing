#include "vector3.h"

vector3::vector3():x(0.0), y(0.0), z(0.0){}

vector3::vector3(double _x, double _y, double _z):x(_x), y(_y), z(_z){}

vector3::vector3(const vector3& v):x(v.x), y(v.y), z(v.z){}

vector3::~vector3(){}

double vector3::length(){
	return (double)sqrt(x*x + y*y + z*z);
}

void vector3::unitization(){
	double l=1.0/sqrtf(x*x + y*y + z*z);
	x*=l;
	y*=l;
	z*=l;
}

vector3 operator - (const vector3& a, const vector3& b) {
	return vector3(a.x-b.x, a.y-b.y, a.z-b.z); 
}

vector3 operator + (const vector3& a, const vector3& b) {
	return vector3(a.x+b.x, a.y+b.y, a.z+b.z); 
}

double operator * (const vector3& a, const vector3& b) {
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

vector3 operator * (double f, const vector3& a) { 
	return vector3(a.x * f, a.y * f, a.z * f); 
}

vector3 operator * (const vector3& a, double f) { 
	return vector3(a.x * f, a.y * f, a.z * f); 
}

void vector3::show(){
	cout << x << " " << y << " " << z << endl;
}

double vector3::get(int dim){
	double res = 0.0;
	switch (dim)
	{
	case 0: res = x; break;
	case 1: res = y; break;
	case 2: res = z; break;
	default:
		break;
	}
	return res;
}