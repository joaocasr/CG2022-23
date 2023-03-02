#include "generator.h"

using namespace std;
std::string path = fs::current_path().string();

vector<Triangle> triangulos;
int sizeTriangulos = 0;


int main(int args, char* argv[]) {
	if (strcmp(argv[1], "box") == 0) {
		int units = atoi(argv[2]);
		int grid = atoi(argv[3]);
		char* filename = argv[4];
		buildCube(units, grid, filename);
	}
	else if (strcmp(argv[1], "plane") == 0) {
		int units = atoi(argv[2]);
		int divs = atoi(argv[3]);
		char* filename = argv[4];
		buildPlane(units, divs, filename);
	}
	else if (strcmp(argv[1], "sphere") == 0) {
		float radius = atof(argv[2]);
		int slices = atoi(argv[3]);
		int stacks = atoi(argv[4]);
		char* filename = argv[5];
		buildSphere(radius,slices,stacks,filename);
	}
	else if (strcmp(argv[1], "cone") == 0) {
		float radius = atof(argv[2]);
		int height = atoi(argv[3]);
		int slices = atoi(argv[4]);
		int stacks = atoi(argv[5]);
		char* filename = argv[6];
		buildCone(radius, height, slices, stacks, filename);
	}
	else {
		std::cout << argv[0] << "\n";
	}
}

void buildPlane(int units, int divs, char* filename) {
	string dir = path.substr(0, path.length() - 13);
	dir += "models\\";
	dir += filename;
	//C:\Users\joaop\Desktop\CG2022-23\FASE1\generator_module\build\Release
	std::cout << dir << "\n";

	float stepx, stepy, stepz;
	stepx = stepy = stepz = static_cast<float>(units) / divs;
	float finalx, finaly, finalz;
	finalx = finaly = finalz = units / static_cast<float>(2);

	//valores das coordenadas dos pontos do triangulos do plano
	float px1, py1, pz1;
	float px2, py2, pz2;
	Point* p1, * p2, * p3, * p4;
	Triangle* t1, * t2;

	for (int i = 0; -finalz + i * stepz < finalz; i++) {
		for (int j = 0; -finalx + j * stepx < finalx; j++) {

			px1 = -finalx + (j * stepx);
			px2 = -finalx + ((j + 1) * stepx);
			pz1 = -finalz + (i * stepz);
			pz2 = -finalz + ((i + 1) * stepz);

			py1 = 0;
			py2 = 0;

			//pontos do plano
			p1 = new Point(px1, py1, pz1);
			p2 = new Point(px2, py1, pz1);
			p3 = new Point(px1, py1, pz2);
			p4 = new Point(px2, py1, pz2);

			//triangulos do plano
			t1 = new Triangle(p1, p3, p4);
			t2 = new Triangle(p1, p4, p2);

			triangulos.push_back(t1);
			triangulos.push_back(t2);
			sizeTriangulos += 2;

		}
	}

	ofstream file;
	file.open(dir);

	if (!file) {
		std::cout << "Error opening file" << "\n";
		exit(1);
	}
	else {
		std::cout << dir << "\n";
	}

	file << to_string(triangulos.size() * 3) << "\n";
	cout << "Writing: <" << to_string(triangulos.size() * 3) << "> Points!" << endl;

	for (int c = 0; c < triangulos.size(); c++) {
		file << TriangleToString(&triangulos[c]);
		cout << "Wrote: " << TriangleToString(&triangulos[c]);
	}

	file.close();
}

void buildCube(int units, int grid, char * filename) {
	std::string dir = path.substr(0, path.length() - 13);
	dir += "models\\";
	dir += filename;
	std::cout << dir << "\n";

	float stepx, stepy, stepz;
	stepx = stepy = stepz = static_cast<float>(units) / grid;
	float finalx, finaly, finalz;
	finalx = finaly = finalz = units / static_cast<float>(2);

	//valores das coordenadas dos pontos do triangulos do plano da frente e de trás
	float px1, py1, pz1;
	float px2, py2, pz2;
	Point* p1, * p2, * p3, * p4, * p5, * p6, * p7, * p8;
	Triangle* t1, * t2, * t3, * t4;
	//para z constante
	//vamos desenhar os dois planos paralelos que são perpendiculares ao eixo OZ
	for (int i = 0; -finalx + i * stepx < finalx; i++) {
		for (int j = 0; -finaly + j * stepy < finaly; j++) {

			px1 = -finalx + (i * stepx);
			px2 = -finalx + ((i + 1) * stepx);
			py1 = -finaly + (j * stepy);
			py2 = -finaly + ((j + 1) * stepy);
			//plano de trás
			pz1 = -finalz;
			//plano de frente
			pz2 = finalz;

			//pontos de um quadrado do plano de trás
			p1 = new Point(px1, py1, pz1);
			p3 = new Point(px2, py1, pz1);
			p2 = new Point(px1, py2, pz1); //ponto de cima   
			p4 = new Point(px2, py2, pz1); //ponto de cima  

			//pontos de um quadrado do plano de frente
			p5 = new Point(px1, py1, pz2);
			p7 = new Point(px2, py1, pz2);
			p6 = new Point(px1, py2, pz2); //ponto de cima 
			p8 = new Point(px2, py2, pz2); //ponto de cima  


			//triangulos do plano de tras
			t1 = new Triangle(p3, p1, p2);//triangulo inferior
			t2 = new Triangle(p3, p2, p4);//trinangulo superior

			//triangulo do plano de frente
			t3 = new Triangle(p5, p7, p8);
			t4 = new Triangle(p5, p8, p6);

			triangulos.push_back(t3);
			triangulos.push_back(t4);
			triangulos.push_back(t1);
			triangulos.push_back(t2);
			sizeTriangulos += 4;
		}
	}

	for (int i = 0; -finalz + i * stepz < finalz; i++) {
		for (int j = 0; -finalx + j * stepx < finalx; j++) {

			px1 = -finalx + (j * stepx);
			px2 = -finalx + ((j + 1) * stepx);
			pz1 = -finalz + (i * stepz);
			pz2 = -finalz + ((i + 1) * stepz);

			py1 = -finaly;
			py2 = finaly;

			//pontos do quadro do plano inferior
			p1 = new Point(px1, py1, pz1);
			p3 = new Point(px2, py1, pz1);
			p5 = new Point(px1, py1, pz2);
			p7 = new Point(px2, py1, pz2);

			//pontos do quadro do plano superior
			p2 = new Point(px1, py2, pz1);
			p4 = new Point(px2, py2, pz1);
			p6 = new Point(px1, py2, pz2);
			p8 = new Point(px2, py2, pz2);

			//triangulos do plano perpendicular ao semieixo negativo Oy
			t1 = new Triangle(p7, p5, p1);
			t2 = new Triangle(p3, p7, p1);

			//triangulo do plano de frente
			t3 = new Triangle(p6, p8, p2);
			t4 = new Triangle(p8, p4, p2);

			triangulos.push_back(t3);
			triangulos.push_back(t4);
			triangulos.push_back(t1);
			triangulos.push_back(t2);
			sizeTriangulos += 4;
		}
	}
	for (int i = 0; -finalz + i * stepz < finalz; i++) {
		for (int j = 0; -finaly + j * stepy < finaly; j++) {

			pz1 = -finalz + (i * stepz);
			pz2 = -finalz + ((i + 1) * stepz);
			py1 = -finaly + (j * stepy);
			py2 = -finaly + ((j + 1) * stepy);
			//plano de trás
			px1 = -finalx;
			//plano de frente
			px2 = finalx;

			//pontos de um quadrado do plano de trás
			p1 = new Point(px1, py1, pz1);
			p2 = new Point(px1, py2, pz1);
			p5 = new Point(px1, py1, pz2);
			p6 = new Point(px1, py2, pz2);

			p3 = new Point(px2, py1, pz1);
			p4 = new Point(px2, py2, pz1);
			p7 = new Point(px2, py1, pz2);
			p8 = new Point(px2, py2, pz2);

			//triangulos do plano perpendicular ao semieixo negativo Ox
			t1 = new Triangle(p1, p5, p6);
			t2 = new Triangle(p1, p6, p2);

			//triangulos do plano perpendicular ao semieixo positivo Ox
			t3 = new Triangle(p7, p3, p8);
			t4 = new Triangle(p3, p4, p8);

			triangulos.push_back(t3);
			triangulos.push_back(t4);
			triangulos.push_back(t1);
			triangulos.push_back(t2);
			sizeTriangulos += 4;
		}
	}

	ofstream file;
	file.open(dir);

	if (!file) {
		std::cout << "Error opening file" << "\n";
		exit(1);
	}
	else {
		std::cout << dir << "\n";
	}

	file << to_string(triangulos.size() * 3) << "\n";
	cout << "Writing: <" << to_string(triangulos.size() * 3) << "> Points!" << endl;

	for (int c = 0; c < triangulos.size(); c++) {
		file << TriangleToString(&triangulos[c]);
		cout << "Wrote: " << TriangleToString(&triangulos[c]);
	}

	file.close();
}

void buildSphere(float radius, int slices, int stacks, const char* filename)
{
	std::string dir = path.substr(0, path.length() - 13);
	dir += "models\\";
	dir += filename;
	std::cout << dir << "\n";

	Point* p1, * p2, * p3, * p4;
	Triangle* t1, * t2;
	double pi = M_PI;

	float deltaAlpha = 2 * pi / slices;
	float deltaBeta = pi / stacks;

	for (int i = -(stacks / 2); i < (stacks / 2); i++) {
		float beta = i * deltaBeta;
		float nextBeta = (i + 1) * deltaBeta;
		
		for (int j = 0; j < slices; j++) {
			float alpha = j * deltaAlpha;
			float nextAlpha = (j + 1) * deltaAlpha;

			p1 = new Point(radius * cos(beta) * sin(alpha), radius * sin(beta), radius * cos(beta) * cos(alpha));
			p2 = new Point(radius * cos(beta) * sin(nextAlpha), radius * sin(beta), radius * cos(beta) * cos(nextAlpha));
			p3 = new Point(radius * cos(nextBeta) * sin(alpha), radius * sin(nextBeta), radius * cos(nextBeta) * cos(alpha));
			p4 = new Point(radius * cos(nextBeta) * sin(nextAlpha), radius * sin(nextBeta), radius * cos(nextBeta) * cos(nextAlpha));

			t1 = new Triangle(p1, p2, p4);
			t2 = new Triangle(p1, p4, p3);

			triangulos.push_back(t1);
			triangulos.push_back(t2);
			sizeTriangulos += 2;
		}
	}

	ofstream file;
	file.open(dir);

	if (!file) {
		std::cout << "Error opening file" << "\n";
		exit(1);
	}
	else {
		std::cout << dir << "\n";
	}

	file << to_string(triangulos.size() * 3) << "\n";
	cout << "Writing: <" << to_string(triangulos.size() * 3) << "> Points!" << endl;

	for (int c = 0; c < triangulos.size(); c++) {
		file << TriangleToString(&triangulos[c]);
		cout << "Wrote: " << TriangleToString(&triangulos[c]);
	}

	file.close();
}


void buildCone(float radius, int height, int slices, int stacks, const char* filename)
{
	// Open the output file
	std::string dir = path.substr(0, path.length() - 13);
	dir += "models\\";
	dir += filename;

	float step = (M_PI * 2) / slices;
	float stackStep = static_cast<float>(height) / static_cast<float>(stacks);
	float alpha = 0.0f;
	Point* p1, * p2, * p3, * p4;

	for (int c = 0; c < slices; c++) {
		//Lower circle
		p1 = new Point(radius * sin(alpha + step), 0.0f, radius * cos(alpha + step));
		p2 = new Point(radius * sin(alpha), 0.0f, radius * cos(alpha));
		p3 = new Point(0.0f, 0.0f, 0.0f);

		triangulos.push_back(new Triangle(p1, p2, p3));

		alpha += step;
	}

	alpha = 0.0f;
	float curHeight = 0.0f;

	for (int i = 0; i < slices; i++) {
		for (int j = 0; j < stacks-1; j++) {
			p1 = new Point(radius * sin(alpha), curHeight, radius * cos(alpha));
			p2 = new Point(radius * sin(alpha + step), curHeight, radius * cos(alpha + step));
			p3 = new Point(radius * sin(alpha + step), curHeight + stackStep, radius * cos(alpha + step));
			p4 = new Point(radius * sin(alpha), curHeight + stackStep, radius * cos(alpha));

			triangulos.push_back(new Triangle(p1, p2, p3));
			triangulos.push_back(new Triangle(p4, p1, p2));

			curHeight += stackStep;
		}

		alpha += step;
	}

	alpha = 0.0f;

	for (int c = 0; c < slices; c++) {
		p1 = new Point(radius * sin(alpha + step), curHeight, radius * cos(alpha + step));
		p2 = new Point(radius * sin(alpha), curHeight, radius * cos(alpha));
		p3 = new Point(0.0f, height, 0.0f);

		triangulos.push_back(new Triangle(p3, p2, p1));

		alpha += step;
	}
	
	ofstream file;
	file.open(dir);

	if (!file) {
		std::cout << "Error opening file" << "\n";
		exit(1);
	}
	else {
		std::cout << dir << "\n";
	}

	file << to_string(triangulos.size() * 3) << "\n";
	cout << "Writing: <" << to_string(triangulos.size() * 3) << "> Points!" << endl;

	for (int c = 0; c < triangulos.size(); c++) {
		file << TriangleToString(&triangulos[c]);
		cout << "Wrote: " << TriangleToString(&triangulos[c]);
	}

	file.close();
}


std::string PointToString(Point p) {
	return to_string(p.x) + "," + to_string(p.y) + "," + to_string(p.z);
}


std::string TriangleToString(Triangle* t) {
	return PointToString(t->p1) + "\n" + PointToString(t->p2) + "\n" + PointToString(t->p3) + "\n";
}
