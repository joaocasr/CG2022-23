#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <IL/il.h>

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
void loadTexture(std::string texFile, int texIndex);

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

class Model {

public:
	string modelo;
	string textureimg = "none";
	vector<float> pontos;
	vector<float> normais;
	vector<float> tex;
	float ambient[4] = { 50.0,50.0,50.0,1.0 };
	float diffuse[4] = { 200.0,200.0,200.0,1.0 };
	float specular[4] = { 0.0,0.0,0.0,1.0 };
	float emissive[4] = { 0.0,0.0,0.0,1.0 };
	float shininess = 0;
	int bufIndex = -1;
	int texIndex = -1;

	Model(string nome) {
		modelo = nome;
	}

	string getNameModel() {
		return modelo;
	}

	void setDRGB(float dr, float dg, float db) {
		diffuse[0] = dr / 255.0f;
		diffuse[1] = dg / 255.0f;
		diffuse[2] = db / 255.0f;
		diffuse[3] = 1.0f;
	}

	void setARGB(float ar, float ag, float ab) {
		ambient[0] = ar / 255.0f;
		ambient[1] = ag / 255.0f;
		ambient[2] = ab / 255.0f;
		ambient[3] = 1.0f;
	}

	void setSRGB(float sr, float sg, float sb) {
		specular[0] = sr / 255.0f;
		specular[1] = sg / 255.0f;
		specular[2] = sb / 255.0f;
		specular[3] = 1.0f;
	}

	void setERGB(float er, float eg, float eb) {
		emissive[0] = er / 255.0f;
		emissive[1] = eg / 255.0f;
		emissive[2] = eb / 255.0f;
		emissive[3] = 1.0f;
	}

	void setShiny(float shiny) {
		shininess = shiny;
	}

	void setTextureImg(string img) {
		textureimg = img;
	}

	void addPointModel(float p) {
		pontos.push_back(p);
	}

	void addPointNormal(float p) {
		normais.push_back(p);
	}

	void addPointTex(float p) {
		tex.push_back(p);
	}

	int getBufIndex() {
		return bufIndex;
	}

	void setBufIndex(int i) {
		this->bufIndex = i;
	}

	int getTexIndex() {
		return texIndex;
	}

	void setTexIndex(int i) {
		this->texIndex = i;
	}

};

class Group {

public:
	vector<Model> modelos;
	vector<Transformation> transformacoes;
	vector<Group> groupChild;

	Group(vector<Transformation> trans, vector<Group> gchild) {
		transformacoes = trans;
		groupChild = gchild;
	}

	Group(vector<Transformation> trans) {
		transformacoes = trans;
	}

	vector<Model> getModelos() {
		return modelos;
	}

	vector<Transformation> getTransformations() {
		return transformacoes;
	}

	vector<Group> getChild() {
		return groupChild;
	}

	void addChild(Group child) {
		groupChild.push_back(child);
	}

	void addModel(Model m) {
		modelos.push_back(m);
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

class Light {

public:
	string type;
	float posLightx = 0;
	float posLighty = 0;
	float posLightz = 0;
	float dirLightx = 0;
	float dirLighty = 0;
	float dirLightz = 0;
	float cutoff = 0;

	Light(string tipo, float posx, float posy, float posz, float dirx, float diry, float dirz, float cutff) { //SPOT
		type = tipo;
		posLightx = posx;
		posLighty = posy;
		posLightz = posz;
		dirLightx = dirx;
		dirLighty = diry;
		dirLightz = dirz;
		cutoff = cutff;
	}

	Light(string tipo, float posx, float posy, float posz) { //POINT
		type = tipo;
		posLightx = posx;
		posLighty = posy;
		posLightz = posz;
	}

	Light(float dirx, float diry, float dirz, string tipo) { //DIRECTIONAL
		type = tipo;
		dirLightx = dirx;
		dirLighty = diry;
		dirLightz = dirz;
	}

};

string getTransformacoes(vector<Transformation> t);
string getModelos(vector<string> modelos);
void build_groups(vector<Group> groups);
