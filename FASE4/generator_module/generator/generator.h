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
	float normX;
	float normY;
	float normZ;
	float texX;
	float texY;

	Point()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		normX = 0.0f;
		normY = 0.0f;
		normZ = 0.0f;
		texX = 0.0f;
		texX = 0.0f;
	}
	Point(float px, float py, float pz)
	{
		x = px;
		y = py;
		z = pz;
		normX = 0.0f;
		normY = 0.0f;
		normZ = 0.0f;
		texX = 0.0f;
		texY = 0.0f;
	}
	Point(float px, float py, float pz, float nx, float ny, float nz, float tx, float ty)
	{
		x = px;
		y = py;
		z = pz;
		normX = nx;
		normY = ny;
		normZ = nz;
		texX = tx;
		texY = ty;
	}
	Point(Point* p) {
		x = p->x;
		y = p->y;
		z = p->z;
		normX = p->normX;
		normY = p->normY;
		normZ = p->normZ;
		texX = p->texX;
		texY = p->texY;
	}

	void matOp(float m, Point p) {
		x += p.x * m;
		y += p.y * m;
		z += p.z * m;
	}

	void setNormal(float nx, float ny, float nz) {
		normX = nx;
		normY = ny;
		normZ = nz;
	}
	void setNormal(float normal[3]) {
		normX = normal[0];
		normY = normal[1];
		normZ = normal[2];
	}
	void setTex(float tx, float ty) {
		texX = tx;
		texY = ty;
	}
};

void multMatrixVector(Point* m, float* vec, Point* res);
void calcAMat(float* m, Point* points, Point* res);
Point multVects(Point u[4], float v[4]);
void cross(float* a, float* b, float* res);
void cross(Point* a, Point* b, float* res);
void normalize(float* a);
void normalize(Point *p);

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