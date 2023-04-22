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
#include <cmath>
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

class Transformation {

public:
	string transformation_name;
	float trsx;
	float trsy;
	float trsz;
	float angle = 0;

	Transformation() {
		transformation_name = "";
		trsx = 0;
		trsy = 0;
		trsz = 0;
		angle = 0;
	}
	Transformation(string nome,float trsxx, float trsyy, float trszz)
	{
		transformation_name = nome;
		trsx = trsxx;
		trsy = trsyy;
		trsz = trszz;
	}
	Transformation(Transformation *t)
	{
		transformation_name = t->transformation_name;
		trsx = t->trsx;
		trsy = t->trsy;
		trsz = t->trsz;
	}
	void setAngle(float a) {
		angle = a;
	}
};

string TransformationToString(Transformation t);

class Group {

public:
	vector<float> modelos;
	vector<Transformation> transformacoes;
	vector<Group> groupChild;

	Group(vector<Transformation> trans, vector<float> models, vector<Group> gchild) {
		transformacoes = trans;
		modelos = models;
		groupChild = gchild;
	}

	Group(vector<Transformation> trans, vector<float> models) {
		transformacoes = trans;
		modelos = models;
	}

	vector<float> getModelos() {
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
};

Group getGroups(tinyxml2::XMLElement* xmlelement, bool top_lvl);
string getTransformacoes(vector<Transformation> t);
string getModelos(vector<string> modelos);