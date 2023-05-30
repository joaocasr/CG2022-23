#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
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
#include "tinyxml2.h"
using std::ofstream;
namespace fs = std::filesystem;
using namespace std;

void parse_XML(std::string xmlfile);

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


class Transformation {

public:
	string transformation_name;
	float trsx;
	float trsy;
	float trsz;
	float angle = 0;
	vector<float> curvepoints;
	float time = -1;
	bool align;

	Transformation() {
		transformation_name = "";
		trsx = 0;
		trsy = 0;
		trsz = 0;
		angle = 0;
	}
	Transformation(string nome, float trsxx, float trsyy, float trszz, float tempo, bool alinhar)
	{
		transformation_name = nome;
		trsx = trsxx;
		trsy = trsyy;
		trsz = trszz;
		align = alinhar;
		time = tempo;
	}
	Transformation(Transformation* t)
	{
		transformation_name = t->transformation_name;
		trsx = t->trsx;
		trsy = t->trsy;
		trsz = t->trsz;
	}
	void setAngle(float a) {
		angle = a;
	}
	void addPoint(float p) {
		curvepoints.push_back(p);
	}
	void setTime(float t) {
		time = t;
	}
	void setAlign(bool b) {
		align = b;
	}

};

string TransformationToString(Transformation t);

class Group {

public:
	vector<float> modelos;
	vector<Transformation> transformacoes;
	vector<Group> groupChild;
	int bufIndex = -1;

	Group(vector<Transformation> trans, vector<Group> gchild) {
		transformacoes = trans;
		groupChild = gchild;
	}

	Group(vector<Transformation> trans) {
		transformacoes = trans;
	}

	vector<float> getModelos() {
		return modelos;
	}

	vector<Transformation> getTransformations() {
		return transformacoes;
	}

	int getBufIndex() {
		return bufIndex;
	}

	void setBufIndex(int i) {
		this->bufIndex = i;
	}

	vector<Group> getChild() {
		return groupChild;
	}

	void addChild(Group child) {
		groupChild.push_back(child);
	}

	void addPointModel(float p) {
		modelos.push_back(p);
	}
};

Group getGroups(tinyxml2::XMLElement* xmlelement, bool top_lvl);

class Primitiva {

public:
	vector<Point> pontos;
	string filename;

	Primitiva(string name, vector<Point> points) {
		pontos = points;
		filename = name;
	}
};


string getTransformacoes(vector<Transformation> t);
string getModelos(vector<string> modelos);
void build_groups(vector<Group> groups);
