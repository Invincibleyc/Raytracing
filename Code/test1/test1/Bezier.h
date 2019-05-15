#ifndef BEZIER_H
#define BEZIER_H
#include "common.h"

class Bezier
{
public:
	int point_number;
	vector3* con_points;

	Bezier();
	~Bezier(void);
	void addpoint(double x, double y, double myscale); //增加控制点
	int factorial(int k);                            //计算阶乘
	int C(int n, int m);                            //计算组合数
	double B(int i, int m, double u);                //计算伯恩斯坦多项式的B_i,m(u)
	vector3 curve(double u, double theta);            //计算曲线上的点

	double dB_du(int i, int m, double u);             //伯恩斯坦多项式的导数

	double f(vector3 xk, Ray myray);   //xk=(t, theta, t)
	double g(vector3 xk, Ray myray);
	double h(vector3 xk, Ray myray);

	double fx(vector3 dir);  //f、g、h对x、y、z的偏导数
	double gx(vector3 dir);
	double hx(vector3 dir);
	
	double fy(vector3 xk);
	double gy(vector3 xk);
	double hy(vector3 xk);

	double fz(vector3 xk);
	double gz(vector3 xk);
	double hz(vector3 xk);

	vector3 interation(vector3 xk, Ray myray);        //牛顿迭代
	vector3 cal_normal(vector3 xk);                  //计算法向
};
#endif