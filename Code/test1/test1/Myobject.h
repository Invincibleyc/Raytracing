#ifndef MYOBJECT_H
#define MYOBJECT_H

#include "Bezier.h"

class Myobject
{
public:
	Myobject(void);
	~Myobject(void);
	virtual vector3 normal(vector3 v) = 0;                   //����
	virtual pair<double, bool> intersect(Ray v) = 0;          //��
	virtual vector3 getColor(vector3 pos=vector3(0,0,0)) = 0;  //�����ɫ
	virtual vector3 cal_light(double k) = 0;
	double getDiffuse();
	double getSpecular();
	double getReflection();
	double getRefractivity();
	bool islight();
	Light getLight();

protected:
	double diffuse;  //������ϵ��
	double specular; //�߹�ϵ��
	double reflect;  //����ϵ��
	double refract;  //����ϵ��
	bool withtexture;//�Ƿ�������
	vector3 color;   //��ɫ
	Mat texture;     //����
	bool light;      //�Ƿ��ǹ�Դ
	Light mylight;    //���Դ
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
	vector3 N;                  //������
	double D;                   //ԭ�㵽ƽ���������������ڷ����������ϵ�ͶӰ

public:
	Plane();
	void init(vector3 _N, double _D);
	Plane(vector3 _N, vector3 _myorigin, vector3 _n_x, vector3 _n_y, double _D, double _diffuse, 
		double _specular, double _reflect, double _refract, vector3 _color, string texturefile="", 
		Light _light = Light());
	vector3 Plane::normal(vector3 v);
	pair<double, bool> intersect(Ray v);
	vector3 getColor(vector3 pos=vector3(0,0,0));
	vector3 myorigin;          //ƽ���ϵ���֪�㣬��Ϊƽ���ԭ�㣨���ڼ�������
	vector3 n_x, n_y;          //y�᷽���x�᷽��
	vector3 cal_light(double k);
	double get_D();
};

class BezierSurface: public Myobject{
private:
	Bezier* myBezier;
	vector3 mynormal;              //���߷���
	vector3** interation_helper;    //������
	vector3** normal_helper;        //������ķ���
	Plane* AABB;                   //��Χ��
public:
	BezierSurface(Bezier* b, double _diffuse, double _specular, double _reflect, double _refract, vector3 _color);
	~BezierSurface();
	vector3 normal(vector3 v);
	pair<double, bool> intersect(Ray v);
	vector3 getColor(vector3 pos=vector3(0,0,0));
	void showobj();  //������������obj�ļ�
	void init_AABB(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z); //��ʼ����Χ��
	double intersect_AABB(Ray v);               //�Ͱ�Χ����
	bool in_AABB(int dir, vector3 cross_point);  //�жϽ����Ƿ��ڰ�Χ�з�Χ��
	vector3 cal_light(double k){return vector3(0, 0, 0);}
};

#endif MYOBJECT_H