#include "Myobject.h"


Myobject::Myobject(void)
{
}


Myobject::~Myobject(void)
{
}

double Myobject::getDiffuse(){
	return diffuse;
}

double Myobject::getSpecular(){
	return specular;
}

double Myobject::getReflection(){
	return reflect;
}

double Myobject::getRefractivity(){
	return refract;
}

bool Myobject::islight(){
	return light;
}

Light Myobject::getLight(){
	return mylight;
}

Sphere::Sphere(vector3 _center, double _radius, double _diffuse, double _specular, 
			   double _reflect, double _refract, vector3 _color, string texturefile): center(_center), radius(_radius){
	diffuse = _diffuse;
	specular = _specular;
	reflect = _reflect;
	refract = _refract;
	color = _color;
	withtexture = false;
	light = false;
	if(texturefile!=""){
		texture = imread(texturefile);
		withtexture = true;
		if(texture.empty()) withtexture = false;
	}
}

vector3 Sphere::normal(vector3 v){
	vector3 _normal = v-center;
	_normal.unitization();
	return _normal;
}

pair<double, bool> Sphere::intersect(Ray v){
	pair<double, bool> res;
	res.second = false;
	double distance = -1.0;
	vector3 l = center - v.origin;
	double t_p = l * v.dir;
	if(l*l >= radius*radius && t_p < 0.0){  //如果光源在球体外部且l和dir夹角大于90度，则没有交点
		res.first = -1.0;
		return res;
	}
	double temp = radius*radius - (l*l - t_p*t_p);
	if(temp > 0.0){  //如果光线与球面相交
		temp = sqrt(temp);
		if(l*l >= radius*radius) distance = t_p - temp;
		else{
			distance = t_p + temp;
			res.second = true;
		}
	}
	res.first = distance;
	return res;  //返回光源到交点的距离（如果没有交点，返回-1）
}

vector3 Sphere::getColor(vector3 pos){
	if(withtexture){
		vector3 temp = pos-center;
		temp = temp*(1/radius);
		double alpha = acos(temp.y);
		double beta;
		if(temp.z == 0.0) beta = pi/2;
		else beta = atan(temp.x/temp.z);
		int x = (int)(texture.cols*beta/(2*pi));
		int y = (int)(texture.rows*alpha/pi);
		while(x < 0) x += texture.cols;
		while(y < 0) y += texture.rows;
		x %= texture.cols;
		y %= texture.rows;
		vector3 res;
		res.x = texture.at<Vec3b>(y, x)[2]/255.0;
		res.y = texture.at<Vec3b>(y, x)[1]/255.0;
		res.z = texture.at<Vec3b>(y, x)[0]/255.0;
		return res;
	}
	else return color;
}

Plane::Plane(){
	diffuse = 0.0;
	specular = 0.0;
	reflect = 0.0;
	refract = 0.0;
	color = vector3(0, 0, 0);
	withtexture = false;
	light = false;
}

void Plane::init(vector3 _N, double _D){
	N = _N;
	D = _D;
}

Plane::Plane(vector3 _N, vector3 _myorigin, vector3 _n_x, vector3 _n_y, double _D, double _diffuse, double _specular, double _reflect, 
			 double _refract, vector3 _color, string texturefile, Light _light): N(_N), D(_D), n_x(_n_x), n_y(_n_y){
	diffuse = _diffuse;
	specular = _specular;
	reflect = _reflect;
	refract = _refract;
	color = _color;
	withtexture = false;
	if(texturefile!=""){
		texture = imread(texturefile);
		withtexture = true;
		if(texture.empty()) withtexture = false;
	}
	mylight = _light;
	if(mylight.center.length() == 0.0) light = false;
	else light = true;
	mylight.center.show();
}

vector3 Plane::normal(vector3 v){
	return N;
}

pair<double, bool> Plane::intersect(Ray ray){
	double distance = -1.0;
	double temp = N * ray.dir;
	if(temp != 0.0){
		distance = -(D + N*ray.origin)/temp;
		if(distance <= 0.0) distance = -1.0;
	}
	pair<double, bool> res;
	res.first = distance;
	res.second = false;
	return res;
}

vector3 Plane::getColor(vector3 pos){
	if(withtexture){
		vector3 res;
		vector3 temp = pos - myorigin;
		double cast_x = temp*n_x;
		double cast_y = temp*n_y;
		int x = (int)cast_x;
		int y = (int)cast_y;
		while(x < 0) x += texture.cols;
		while(y < 0) y += texture.rows;
		x %= texture.cols;
		y %= texture.rows;
		res.x = texture.at<Vec3b>(y, x)[2]/255.0;
		res.y = texture.at<Vec3b>(y, x)[1]/255.0;
		res.z = texture.at<Vec3b>(y, x)[0]/255.0;
		return res;
	}
	else return color;
}

vector3 Plane::cal_light(double k){
	double dist = 3.0;
	double theta = 2.0*pi/(lightnumber/2)*k;
	vector3 res = mylight.center + vector3(0.0, 80.0*scale, 80.0*scale);
	res = res + n_x*cos(theta)*dist*scale + n_y*sin(theta)*dist;
	if(k >= 6 && k < 12) res = res + vector3(0.0, 3.0*scale, 0.0);
	else res = res + vector3(0.0, -3.0*scale, 0.0);
	return res;
}

double Plane::get_D(){
	return D;
}

BezierSurface::BezierSurface(Bezier* b, double _diffuse, double _specular, double _reflect, double _refract, vector3 _color){
	diffuse = _diffuse;
	specular = _specular;
	reflect = _reflect;
	refract = _refract;
	color = _color;
	interation_helper = nullptr;
	myBezier = b;
	light = false;
	showobj();
}

BezierSurface::~BezierSurface(){}

vector3 BezierSurface::normal(vector3 v){
	return mynormal;
}

pair<double, bool> BezierSurface::intersect(Ray v){
	/*swap(v.dir.y, v.dir.z);
	swap(v.origin.y, v.origin.z);*/
	mynormal = vector3(0.0, 0.0, 0.0);
	pair<double, bool> res;
	res.first = -1.0;
	res.second = false;
 
	double dist = intersect_AABB(v);
	if(dist == -1.0) return res;
	double u = 0.0, theta= 0.0;
	double min_L = -1.0;
	for(int i = 1; i < 120; i++){
		for(int j = 1; j < 120; j++){
			vector3 temp = interation_helper[i][j]-v.origin;
			double temp_l = temp*v.dir/temp.length();
			double dot = normal_helper[i][j] * v.dir;
			if(min_L == -1.0){
				u = i;
				theta = j;
				min_L = temp_l;
			}
			else if(min_L < temp_l && dot >= 0){
				min_L = temp_l;
				u = i;
				theta = j;
			}
		}
	}
	u /= 120;
	theta /= 120;
	theta *= 2*pi;
	double t = (myBezier->curve(u, theta) - v.origin).length();
	//t -= 0.005;
	vector3 xk(t, u, theta);
	vector3 xk_F(myBezier->f(xk, v), myBezier->g(xk, v), myBezier->h(xk, v));
	for(int j = 0; j < 25; j++){
		vector3 temp = myBezier->interation(xk, v);
		vector3 temp_F(myBezier->f(temp, v), myBezier->g(temp, v), myBezier->h(temp, v));
		/*xk = temp;
		xk.show();
		temp_F.show();*/
		if(temp_F.length() < xk_F.length() && temp.x > 0 && temp.y < 1 && temp.y > 0 && temp.z < 1 && temp.z > 0){
			xk = temp;
			xk_F = temp_F;
			//cout << "bingo" << endl;
		}
	}
	if(abs(myBezier->f(xk, v)) < 2.1 && abs(myBezier->g(xk, v)) < 2.1 && abs(myBezier->h(xk, v)) < 2.1){
		mynormal = myBezier->cal_normal(xk);
		mynormal.unitization();
		mynormal = mynormal * (-1.0);
		res.first = xk.x;
		return res;
	}
	return res;
}

vector3 BezierSurface::getColor(vector3 pos){
	return color;
}

void BezierSurface::showobj(){
	ofstream outf; 
	outf.open("yc1.obj");
	
	bool ini = false;
	double min_x = -1.0, min_y = -1.0, min_z = -1.0, max_x = -1.0, max_y = -1.0, max_z = -1.0;
	int u = 120, v = 120;
	interation_helper = new vector3*[u+1];
	normal_helper = new vector3*[u+1];
	for(int i = 0; i <= u; i++){
		interation_helper[i] = new vector3[v+1];
		normal_helper[i] = new vector3[v+1];
		for(int j = 0; j <= v; j++){
			double _u = i;
			double _v = j;
			_v *= 2*pi;
			_u /= u;
			_v /= v;
			vector3 hh = myBezier->curve(_u, _v);
			if(!ini){
				ini = true;
				min_x = max_x = hh.x;
				min_y = max_y = hh.y;
				min_z = max_z = hh.z;
			}
			else{
				min_x = min(min_x, hh.x); max_x = max(max_x, hh.x);
				min_y = min(min_y, hh.y); max_y = max(max_y, hh.y);
				min_z = min(min_z, hh.z); max_z = max(max_z, hh.z);
			}
			interation_helper[i][j] = hh;
			normal_helper[i][j] = myBezier->cal_normal(vector3(0.0, _u, _v));
			outf << "v" << " ";
			outf << hh.x << " " << hh.y << " " << hh.z << endl;
		}
	}
	for(int j = 0; j < v; j++){
		for(int i = 0; i < u; i++){
			outf << "f " ;
			outf << (i+1 + j*(u+1)) << " " << (i+2 + j*(u+1)) << " " << (i+1 + (j+1)*(u+1)) << endl;
			outf << "f " ;
			outf << (i+2 + j*(u+1)) << " " << (i+1 + (j+1)*(u+1)) << " " << (i+2 + (j+1)*(u+1)) << endl;
		}
	}
	outf.close();
	init_AABB(min_x-1.0, min_y-1.0, min_z-1.0, max_x+1.0, max_y+1.0, max_z+1.0);
}

void BezierSurface::init_AABB(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z){
	AABB = new Plane[6];   //分别表示左、右、上、下、后、前六个面
	AABB[0].init(vector3(1, 0, 0), -min_x);
	AABB[1].init(vector3(1, 0, 0), -max_x);
	AABB[2].init(vector3(0, 1, 0), -min_y);
	AABB[3].init(vector3(0, 1, 0), -max_y);
	AABB[4].init(vector3(0, 0, 1), -min_z);
	AABB[5].init(vector3(0, 0, 1), -max_z);
}

double BezierSurface::intersect_AABB(Ray v){
	pair<double, bool> res;
	double dist = 1000000.0;
	bool is_intersect = false;
	for(int i = 0; i < 6; i++){
		res = AABB[i].intersect(v);
		double temp_dist = res.first;
		if(temp_dist > 0.0 && dist > temp_dist && in_AABB(i, v.origin + temp_dist*v.dir)){
			dist = temp_dist;
			is_intersect = true;
		}
	}
	if(is_intersect) return dist;
	else return -1.0;
}

bool BezierSurface::in_AABB(int dir, vector3 cross_point){
	double min_x = -AABB[0].get_D();
	double max_x = -AABB[1].get_D();
	double min_y = -AABB[2].get_D();
	double max_y = -AABB[3].get_D();
	double min_z = -AABB[4].get_D();
	double max_z = -AABB[5].get_D();
	if(dir == 0 || dir == 1){
		if(cross_point.y >= min_y && cross_point.y <= max_y && cross_point.z >= min_z && cross_point.z <= max_z)
			return true;
	}
	if(dir == 2 || dir == 3){
		if(cross_point.x >= min_x && cross_point.x <= max_x && cross_point.z >= min_z && cross_point.z <= max_z)
			return true;
	}
	if(dir == 4 || dir == 5){
		if(cross_point.x >= min_x && cross_point.x <= max_x && cross_point.y >= min_y && cross_point.y <= max_y)
			return true;
	}
	return false;
}