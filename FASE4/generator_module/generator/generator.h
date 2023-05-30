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
using namespace std;
using std::ofstream;
namespace fs = std::filesystem;

void buildPlane(int units, int divs, char* filename);
void buildCube(int units, int grid, char* filename);
void buildSphere(float radius, int slices, int stacks, const char* filename);
void buildCone(float radius, int height, int slices, int stacks, const char* filename);
void buildCylinder(float b_rad, float t_rad, int height, int slices, int stacks, const char* filename);
void buildTorus(float rad1, float rad2, int slices, int stacks, char* filename);
void buildTeapot(char* fpatch, int tesLvl, char* filename);

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

	void matOp(float m, Point p) {
		x += p.x * m;
		y += p.y * m;
		z += p.z * m;
	}
};

void multMatrixVector(Point* m, float* vec, Point* res);
void calcAMat(float* m, Point* points, Point* res);
Point multVects(Point u[4], float v[4]);
void cross(float* a, float* b, float* res);
void normalize(float* a);

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