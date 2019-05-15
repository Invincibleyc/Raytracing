#ifndef MYOBJECT_H
#define MYOBJECT_H

#include "Bezier.h"

class Myobject
{
public:
	Myobject(void);
	~Myobject(void);
	virtual vector3 normal(vector3 v) = 0;                   //求法向
	virtual pair<double, bool> intersect(Ray v) = 0;          //求交
	virtual vector3 getColor(vector3 pos=vector3(0,0,0)) = 0;  //获得颜色
	virtual vector3 cal_light(double k) = 0;
	double getDiffuse();
	double getSpecular();
	double getReflection();
	double getRefractivity();
	bool islight();
	Light getLight();

protected:
	double diffuse;  //漫反射系数
	double specular; //高光系数
	double reflect;  //反射系数
	double refract;  //折射系数
	bool withtexture;//是否有纹理
	vector3 color;   //颜色
	Mat texture;     //纹理
	bool light;      //是否是光源
	Light mylight;    //面光源
};

class Sphere: public Myobject{
private:
	vector3 center;
	double radius;

public:
	Sphere(vector3 _center, double _radius, double _diffuse, double _specular, double _reflect,
		double _refract, vector3 _color, string texturefile = "");
	vector3 normal(vector3 v);
	pair<double, bool> intersect(Ray v);
	vector3 getColor(vector3 pos=vector3(0,0,0));
	vector3 cal_light(double k){return vector3(0, 0, 0);}
};

class Plane: public Myobject{
private:
	vector3 N;                  //法向量
	double D;                   //原点到平面上任意点的向量在法向量方向上的投影

public:
	Plane();
	void init(vector3 _N, double _D);
	Plane(vector3 _N, vector3 _myorigin, vector3 _n_x, vector3 _n_y, double _D, double _diffuse, 
		double _specular, double _reflect, double _refract, vector3 _color, string texturefile="", 
		Light _light = Light());
	vector3 Plane::normal(vector3 v);
	pair<double, bool> intersect(Ray v);
	vector3 getColor(vector3 pos=vector3(0,0,0));
	vector3 myorigin;          //平面上的已知点，作为平面的原点（用于计算纹理）
	vector3 n_x, n_y;          //y轴方向和x轴方向
	vector3 cal_light(double k);
	double get_D();
};

class BezierSurface: public Myobject{
private:
	Bezier* myBezier;
	vector3 mynormal;              //法线方向
	vector3** interation_helper;    //采样点
	vector3** normal_helper;        //采样点的法向
	Plane* AABB;                   //包围盒
public:
	BezierSurface(Bezier* b, double _diffuse, double _specular, double _reflect, double _refract, vector3 _color);
	~BezierSurface();
	vector3 normal(vector3 v);
	pair<double, bool> intersect(Ray v);
	vector3 getColor(vector3 pos=vector3(0,0,0));
	void showobj();  //采样，并生成obj文件
	void init_AABB(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z); //初始化包围盒
	double intersect_AABB(Ray v);               //和包围盒求交
	bool in_AABB(int dir, vector3 cross_point);  //判断交点是否在包围盒范围内
	vector3 cal_light(double k){return vector3(0, 0, 0);}
};

#endif MYOBJECT_H