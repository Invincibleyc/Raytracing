#ifndef SCENE_H
#define SCENE_H

#include"Myobject.h"

class Scene
{
public:
	Scene(void);
	~Scene(void);
	int object_number;  //物体数目
	int light_number;   //光源数目
	Myobject** object;
	vector3* light;
	vector3* light_color;
	void add_light(vector3 v, vector3 color);
	void add_object(Myobject* new_object);
	void add_sphere(vector3 _center, double _radius, double _diffuse, double _specular, 
		double _reflect, double _refract, vector3 _color, string texturefile="");
	void add_plane(vector3 _N, vector3 _myorigin, vector3 _n_x, vector3 _n_y, double _D, 
		double _diffuse, double _specular, double _reflect, double _refract, vector3 _color, 
		string texturefile="", Light _light = Light());
	void add_bezier(Bezier* b,  double _diffuse, double _specular, double _reflect, double _refract, vector3 _color);
};

#endif