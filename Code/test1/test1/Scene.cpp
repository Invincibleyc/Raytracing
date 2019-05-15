#include "Scene.h"


Scene::Scene()
{
	object_number = 0;
	object = new Myobject*[100];
	light_number = 0;
	light = new vector3[100];
	light_color = new vector3[20];
}

Scene::~Scene(void)
{
}

void Scene::add_light(vector3 v, vector3 color){
	light[light_number] = v;
	light_color[light_number] = color;
	light_number++;
}

void Scene::add_object(Myobject* new_object){
	object[object_number] = new_object;
	object_number++;
}

void Scene::add_sphere(vector3 _center, double _radius, double _diffuse, double _specular, double _reflect, double _refract, vector3 _color, string texturefile){
	Myobject* new_sphere = new Sphere(_center, _radius, _diffuse, _specular, _reflect, _refract, _color, texturefile);
	add_object(new_sphere);
}

void Scene::add_plane(vector3 _N, vector3 _myorigin, vector3 _n_x, vector3 _n_y, double _D, double _diffuse, double _specular, double _reflect, double _refract, 
					  vector3 _color, string texturefile, Light _light){
	Myobject* new_plane = new Plane(_N, _myorigin, _n_x, _n_y, _D, _diffuse, _specular, _reflect, _refract, _color, texturefile, _light);
	add_object(new_plane);
}

void Scene::add_bezier(Bezier* b,  double _diffuse, double _specular, double _reflect, double _refract, vector3 _color){
	Myobject* new_bezier = new BezierSurface(b, _diffuse, _specular, _reflect, _refract, _color);
	add_object(new_bezier);
}