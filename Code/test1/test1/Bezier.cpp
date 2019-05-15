#include "Bezier.h"


Bezier::Bezier()
{
	point_number = 0;
	con_points = new vector3[100];
}

Bezier::~Bezier(void)
{
}

void Bezier::addpoint(double x, double z, double myscale){
	con_points[point_number].x = x*myscale;
	con_points[point_number].y = 0.0*myscale;
	con_points[point_number].z = z*myscale;
	point_number++;
}

int Bezier::factorial(int k){
	int res = 1;
	for(int i = 1; i <= k; i++){
		res *= i;
	}
	return res;
}

int Bezier::C(int n, int m){
	return factorial(n)/(factorial(m)*factorial(n-m));
}

double Bezier::B(int i, int n, double t){
	return C(n, i)*pow(t, i)*pow(1.0-t, n-i);
}

vector3 Bezier::curve(double u, double theta){
	vector3 res;
	double _x = 0.0, _z = 0.0;
	for(int i = 0; i < point_number; i++){
		double temp = B(i, point_number-1, u);
		_x += con_points[i].x * temp;
		_z += con_points[i].z * temp;
	}
	res.x = _x * cos(theta);
	res.y = _z;
	res.z = _x * sin(theta);

	res.x += 230*scale;
	res.y += 224.3*scale;
	res.z += 250*scale;
	return res;
}

double Bezier::dB_du(int i, int n, double t){
	return C(n, i) * (i*pow(t, i-1)*pow(1.0-t, n-i) - (n-i)*pow(t, i)*pow(1.0-t, n-1-i));
}

double Bezier::f(vector3 xk, Ray r){
	double res = 0.0;
	for(int i = 0; i < point_number; i++){
		res += con_points[i].x * B(i, point_number-1, xk.y);
	}
	res *= cos(xk.z);
	res = r.origin.x + r.dir.x * xk.x - res;

	res -= 230*scale;
	return res;
}

double Bezier::g(vector3 xk, Ray r){
	double res = 0.0;
	for(int i = 0; i < point_number; i++){
		res += con_points[i].z * B(i, point_number-1, xk.y);
	}
	res = r.origin.y + r.dir.y * xk.x - res;

	res -= 224.3*scale;
	return res;
}

double Bezier::h(vector3 xk, Ray r){
	double res = 0.0;
	for(int i = 0; i < point_number; i++){
		res += con_points[i].x * B(i, point_number-1, xk.y);
	}
	res *= sin(xk.z);
	res = r.origin.z + r.dir.z * xk.x - res;

	res -= 250*scale;
	return res;
}

double Bezier::fx(vector3 dir){  //此处应该代入光线的方向dir
	return dir.x;
}

double Bezier::gx(vector3 dir){
	return dir.y;
}

double Bezier::hx(vector3 dir){
	return dir.z;
}

double Bezier::fy(vector3 xk){
	double res = 0.0;
	for(int i = 0; i < point_number; i++){
		res += con_points[i].x * dB_du(i, point_number-1, xk.y);
	}
	res *= cos(xk.z);
	res = -res;
	return res;
}

double Bezier::gy(vector3 xk){
	double res = 0.0;
	for(int i = 0; i < point_number; i++){
		res += con_points[i].z * dB_du(i, point_number-1, xk.y);
	}
	res = -res;
	return res;
}

double Bezier::hy(vector3 xk){
	double res = 0.0;
	for(int i = 0; i < point_number; i++){
		res += con_points[i].x * dB_du(i, point_number-1, xk.y);
	}
	res *= sin(xk.z);
	res = -res;
	return res;
}

double Bezier::fz(vector3 xk){
	double res = 0.0;
	for(int i = 0; i < point_number; i++){
		res += con_points[i].x * B(i, point_number-1, xk.y);
	}
	res *= sin(xk.z);
	return res;
}

double Bezier::gz(vector3 xk){
	return 0.0;
}

double Bezier::hz(vector3 xk){
	double res = 0.0;
	for(int i = 0; i < point_number; i++){
		res += con_points[i].x * B(i, point_number-1, xk.y);
	}
	res *= -cos(xk.z);
	return res;
}

vector3 Bezier::interation(vector3 xk, Ray myray){  //计算x_k+1
	double f_x = fx(myray.dir), f_y = fy(myray.dir), f_z = fz(myray.dir);
	double g_x = gx(xk), g_y = gy(xk), g_z = gz(xk);
	double h_x = hx(xk), h_y = hy(xk), h_z = hz(xk);
	double _f = f(xk, myray), _g = g(xk, myray), _h = h(xk, myray);
	vector3 res;
	double t = f_x*(g_y*h_z-g_z*h_y) - g_x*(f_y*h_z-f_z*h_y) + h_x*(f_y*g_z-f_z*g_y);
	res.x = _f*(g_y*h_z-g_z*h_y) + _g*(f_z*h_y-f_y*h_z) + _h*(f_y*g_z-f_z*g_y);
	res.y = _f*(g_z*h_x-g_x*h_z) + _g*(f_x*h_z-f_z*h_x) + _h*(g_x*f_z-f_x*g_z);
	res.z = _f*(g_x*h_y-g_y*h_x) + _g*(f_y*h_x-f_x*h_y) + _h*(f_x*g_y-g_x*f_y);
	res.x /= t;
	res.y /= t;
	res.z /= t;
	return xk-res;
}

vector3 Bezier::cal_normal(vector3 xk){
	vector3 res, temp;
	for(int i = 0; i < point_number; i++){
		double db_du = dB_du(i, point_number-1, xk.y);
		temp.x += con_points[i].x * db_du;
		temp.z += con_points[i].z * db_du;
	}
	res.x = temp.z * cos(xk.z);
	res.y = -temp.x;
	res.z = temp.z * sin(xk.z);
	return res;
}