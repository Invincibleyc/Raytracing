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
	void addpoint(double x, double y, double myscale); //���ӿ��Ƶ�
	int factorial(int k);                            //����׳�
	int C(int n, int m);                            //���������
	double B(int i, int m, double u);                //���㲮��˹̹����ʽ��B_i,m(u)
	vector3 curve(double u, double theta);            //���������ϵĵ�

	double dB_du(int i, int m, double u);             //����˹̹����ʽ�ĵ���

	double f(vector3 xk, Ray myray);   //xk=(t, theta, t)
	double g(vector3 xk, Ray myray);
	double h(vector3 xk, Ray myray);

	double fx(vector3 dir);  //f��g��h��x��y��z��ƫ����
	double gx(vector3 dir);
	double hx(vector3 dir);
	
	double fy(vector3 xk);
	double gy(vector3 xk);
	double hy(vector3 xk);

	double fz(vector3 xk);
	double gz(vector3 xk);
	double hz(vector3 xk);

	vector3 interation(vector3 xk, Ray myray);        //ţ�ٵ���
	vector3 cal_normal(vector3 xk);                  //���㷨��
};
#endif