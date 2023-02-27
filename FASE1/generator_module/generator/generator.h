#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <list>
#include <fstream>
#include <cstdlib>
#include <filesystem>
#include <math.h>
#define M_PI 3.14159265358979323846
using std::ofstream;
namespace fs = std::filesystem;

void buildPlane(int units, int divs, char* filename);
void buildCube(int units, int grid, char* filename);
void buildSphere(float radius, int slices, int stacks, const char* filename);

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

std::string PointToString(Point p);

std::string TriangleToString(Triangle* t);