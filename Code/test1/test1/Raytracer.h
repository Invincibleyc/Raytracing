#ifndef RAYTRACER_H
#define RAYTRACER_H
#include "common.h"
#include "Scene.h"
#include<opencv2\opencv.hpp>
using namespace cv;

class Raytracer
{
public:
	Raytracer(void);
	~Raytracer(void);
	vector3 raytrace(Ray myray, int depth, double pre_refract);
	Scene* myscene;
	Mat draw();
	int traceback_depth;  //µ›πÈ…Ó∂»
	double temp_distance;
};

#endif