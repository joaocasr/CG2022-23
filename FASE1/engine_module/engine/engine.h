#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include<vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <list>
#include <string>
#include <fstream>
#include <filesystem>
#include <string>
#include <cstdlib>
#include <iostream>
using std::ofstream;
namespace fs = std::filesystem;
using namespace std;

void parse_XML(std::string xmlfile);
void build_models();

class Point {

public:
	float x;
	float y;
	float z;
	int dim;

	Point()
	{
		x = 0;
		y = 0;
		z = 0;
		dim = 0;
	}

	Point(float px, float py, float pz, int pdim)
	{
		x = px;
		y = py;
		z = pz;
		dim = pdim;
	}

	Point(Point* p) {
		x = p->x;
		y = p->y;
		z = p->z;
		dim = p->dim;
	}
};

class Triangle {

public:
	Point p1;
	Point p2;
	Point p3;

	Triangle() {
		p1 = new Point();
		p2 = new Point();
		p3 = new Point();
	}
	Triangle(Point* point1, Point* point2, Point* point3)
	{
		p1 = new Point(point1);
		p2 = new Point(point2);
		p3 = new Point(point3);
	}
	Triangle(Triangle* t)
	{
		p1 = t->p1;
		p2 = t->p2;
		p3 = t->p3;
	}
};