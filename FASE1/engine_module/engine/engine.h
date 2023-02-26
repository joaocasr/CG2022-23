#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include<vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <list>
#include <string>
#include <fstream>
using std::ofstream;
#include <cstdlib>

#include <iostream>
using namespace std;
void parse_XML();

class Point {

public:
	float x;
	float y;
	float z;

	Point()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	Point(float px, float py, float pz)
	{
		x = px;
		y = py;
		z = pz;
	}
	Point(Point* p) {
		x = p->x;
		y = p->y;
		z = p->z;
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

