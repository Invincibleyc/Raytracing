#include "Raytracer.h"

Raytracer::Raytracer(void){
	myscene = new Scene();
	traceback_depth = 1;
	temp_distance = 0.0;

	myscene->add_sphere(vector3(160*scale, 313*scale, 300*scale), 37.0*scale, 
		0.0, 0.3, 0.55, 0.0, vector3(255.0/255.0, 255.0/255.0, 255.0/255.0));
	myscene->add_sphere(vector3(370*scale, 324*scale, 250*scale), 26.0*scale,
		0.3, 0.6, 0.6, 1.23, vector3(5.0/255.0, 5.0/255.0, 5.0/255.0));
	myscene->add_sphere(vector3(290*scale, 320*scale, 260*scale), 30.0*scale,
		0.85, 0.25, 0.01, 0.0, vector3(5.0/255.0, 5.0/255.0, 5.0/255.0), "marble4.jpg");
	myscene->add_plane(vector3(0, 0, 1), vector3(0, 0, 220), vector3(1, 0, 0), vector3(0, -1, 0), -150.0*scale,
		0.8, 0.2, 0.0, 0.0, vector3(255.0/255.0, 255.0/255.0, 255.0/255.0), "wall5.bmp");  //后面的平面
	myscene->add_plane(vector3(0, -1, 0), vector3(0, 350, 0), vector3(1, 0, 0), vector3(0, 0, 1), 350.0*scale,
		0.8, 0.6, 0.2, 0.0, vector3(255.0/255.0, 255.0/255.0, 255.0/255.0), "floor1.bmp");  //下面的平面
	myscene->add_plane(vector3(0, 1, 0), vector3(130, 0, 0), vector3(1, 0, 0), vector3(0, 0, -1), -130.0*scale, 
		0.8, 0.2, 0.0, 0.0, vector3(255.0/255.0, 215.0/255.0, 0.0/255.0), "pattern5.jpg", 
		Light(vector3(260*scale, 130*scale, 290*scale), vector3(1.0, 1.0, 1.0), 30.0*scale));  //上面的平面
	
	//***以下为贝塞尔曲线旋转体
	Bezier* b = new Bezier;
	b->addpoint(3.5/1.1, 58.0/1.1, scale);
	b->addpoint(4.0/1.1, 48.0/1.1, scale);
	b->addpoint(5.0/1.1, 43.0/1.1, scale);
	b->addpoint(30.0/1.1, 35.0/1.1, scale);
	b->addpoint(25.0/1.1, 18.0/1.1, scale);
	b->addpoint(20.0/1.1, 0.0/1.1, scale);
	myscene->add_bezier(b, 0.3, 0.4, 0.4, 0.0, vector3(255.0/255.0, 215.0/255.0, 0.0/255.0));

	Bezier* b1 = new Bezier;
	b1->addpoint(25.0/1.1, 138.0/1.1, scale);
	b1->addpoint(15.0/1.1, 134.0/1.1, scale);
	b1->addpoint(8.0/1.1, 130.0/1.1, scale);
	b1->addpoint(3.0/1.1, 128.0/1.1, scale);
	b1->addpoint(3.5/1.1, 123.0/1.1, scale);
	b1->addpoint(3.5/1.1, 73.0/1.1, scale);
	b1->addpoint(3.5/1.1, 58.0/1.1, scale);
	myscene->add_bezier(b1, 0.3, 0.4, 0.4, 0.0, vector3(255.0/255.0, 215.0/255.0, 0.0/255.0));
	//***以上为贝塞尔曲线旋转体
}


Raytracer::~Raytracer(void)
{
}

vector3 Raytracer::raytrace(Ray myray, int depth, double pre_refract){
	vector3 color(0, 0, 0);
	double distance = 1000000.0;             //记录光源出发点到交点的最短距离
	bool inside_object = false;              //判断光线出发点是否在物体内部
	double object_num = -1;                  //添加阴影时，判断是否与自身相交，避免出现黑斑点
	Myobject* first_object = myscene->object[0];
	for(int i = 0; i < myscene->object_number; i++){
		pair<double, bool> temp =  myscene->object[i]->intersect(myray);
		if(temp.first > 0.0 && distance > temp.first){
			distance = temp.first;
			inside_object = temp.second;
			first_object = myscene->object[i];
			object_num = i;
		}
	}
	if(object_num == -1) return vector3(0, 0, 0);
	temp_distance = distance;
	vector3 cross_point = myray.origin + myray.dir*distance;

	if(first_object->islight()){
		if((cross_point - first_object->getLight().center).length() < first_object->getLight().radius){
			return first_object->getLight().color;
		}
	}
	vector3 N = first_object->normal(cross_point);
	double diffuse = first_object->getDiffuse();      //漫反射系数
	double specular = first_object->getSpecular();    //反射系数
	double reflect = first_object->getReflection();   //镜面反射系数
	double refract = first_object->getRefractivity(); //折射系数
	vector3 c1 = first_object->getColor(cross_point);
	vector3 c2(1.0, 1.0, 1.0);
	for(int i = 0; i < myscene->object_number; i++){
		if(myscene->object[i]->islight()){
			srand((unsigned int)time(NULL));
			for(int j = 0; j < lightnumber; j++){
				vector3 light = myscene->object[i]->cal_light(j);
				c2 = myscene->object[i]->getLight().color;
				vector3 L = light-cross_point;
				L.unitization();
				double dot = N*L;
				vector3 R = L-dot*2.0*N;

				//添加阴影
				double shade = 1.0;
				vector3 cross_point_to_light = light-cross_point;
				double len = cross_point_to_light.length();
				cross_point_to_light.unitization();
				Ray ray = Ray(cross_point, cross_point_to_light);
				for(int i = 0; i < myscene->object_number; i++){
					if(i == object_num) continue;
					pair<double, bool> temp1 = myscene->object[i]->intersect(ray);
					if(myscene->object[i]->islight()) continue;
					if(temp1.first>0.0 && temp1.first < len){
						shade = 0.35f;
						break;
					}
				}
				if(dot > 0){
					//漫反射
					double diff = dot*diffuse*shade;
					color.x += diff*c1.x*c2.x/lightnumber;
					color.y += diff*c1.y*c2.y/lightnumber;
					color.z += diff*c1.z*c2.z/lightnumber;

					//普通反射
					double ref = R*myray.dir;
					if(ref > 0){
						double temp = 1.0;
						int n = 8;
						for(int i = 0; i < n; i++){
							temp *= ref;
						}
						specular*=shade;
						color.x += specular*temp*c1.x/lightnumber;
						color.y += specular*temp*c1.y/lightnumber;
						color.z += specular*temp*c1.z/lightnumber;
					}
				} 
			}
		}
	}
	//镜面反射
	if(reflect>0.0){
		if(depth < traceback_depth){
			vector3 _R = myray.dir - 2.0*myray.dir*N*N;
			_R.unitization();
			vector3 trace_color = raytrace(Ray(cross_point+_R*0.01, _R), depth+1, refract);
			color.x += reflect*trace_color.x*c1.x;
			color.y += reflect*trace_color.y*c1.x;
			color.z += reflect*trace_color.z*c1.x;
		}
	}


	//折射
	if(inside_object) N = N*(-1.0);
	if(refract>0.0){
		if(depth < 6){
			double cos_theta_i = -1.0*myray.dir*N;
			double cos_theta_t_2 = 1.0-(pre_refract*pre_refract)*(1-cos_theta_i*cos_theta_i)/(refract*refract);
			if(cos_theta_t_2 > 0.0){
				double cos_theta_t = sqrt(cos_theta_t_2);
				vector3 T = (pre_refract/refract)*myray.dir + ((pre_refract/refract)*cos_theta_i - cos_theta_t)*N;
				vector3 trace_color = raytrace(Ray(cross_point+T*0.001, T), depth+1, refract);
				color.x += trace_color.x;
				color.y += trace_color.y;
				color.z += trace_color.z;
			}
		}
	}


	return color;
}

Mat Raytracer::draw(){
	int m_Width = 500*scale;
	int m_Height = 500*scale;

	Mat img = Mat::zeros(m_Width,m_Height,CV_8UC3);
	vector3 o( 265*scale, 270*scale, 750*scale);  //观察点
	for ( int y = 0; y < m_Width; y++ )  //似乎是扫描线法绘制
	{
		cout << y << endl;
		for ( int x = 0; x < m_Height; x++ )
		{
			vector3 acc( 0, 0, 0 );
			vector3 dir = vector3(x, y, 0) - o;
			dir.unitization();
			Ray r(o, dir);
			double dist;
			acc = raytrace(r, 0, 1.0);    //prim并没有用到
			int red = (int)(acc.x * 256);
			int green = (int)(acc.y * 256);
			int blue = (int)(acc.z * 256);
			if (red > 255) red = 255;
			if (green > 255) green = 255;
			if (blue > 255) blue = 255;
			img.at<Vec3b>(y,x)[0] = blue; //B  
			img.at<Vec3b>(y,x)[1] = green; //G 
			img.at<Vec3b>(y,x)[2] = red; //R
		}
	}
	return img;
}