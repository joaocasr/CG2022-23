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
	else {
		std::cout << argv[0] << "\n";
	}
}

void buildPlane(int units, int divs, char* filename) {

	
	std::string dir = path.substr(0, path.length() - 13);
	dir += "models\\";
	dir += filename;
	//C:\Users\joaop\Desktop\CG2022-23\FASE1\generator_module\build\Release
	std::cout << dir << "\n";
	ofstream file;
	file.open(dir);

	if (!file) {
		std::cout << "Error opening file" << "\n";
		exit(1);
	}
	else {
		std::cout << dir << "\n";
	}


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

			py1 = -finaly;
			py2 = finaly;

			//pontos do plano
			p1 = new Point(px1, py1, pz1);
			p2 = new Point(px2, py1, pz1);
			p3 = new Point(px1, py1, pz2);
			p4 = new Point(px2, py1, pz2);


			//triangulos do plano
			t1 = new Triangle(p4, p3, p1);
			t2 = new Triangle(p2, p4, p1);

			triangulos.push_back(t1);
			triangulos.push_back(t2);
			sizeTriangulos += 2;
;
			file << TriangleToString(t1) << endl;
			file << TriangleToString(t2) << endl;

		}
	}
	file.close();
	std::cout << "size:" << sizeTriangulos << "\n";

	for (Triangle t : triangulos) {
		std::cout << std::to_string(t.p1.x) + ";" + std::to_string(t.p1.y) + ";" + std::to_string(t.p1.z) << ",";
		std::cout << std::to_string(t.p2.x) + ";" + std::to_string(t.p2.y) + ";" + std::to_string(t.p2.z) << ",";
		std::cout << std::to_string(t.p3.x) + ";" + std::to_string(t.p3.y) + ";" + std::to_string(t.p3.z) << "\n";
	}
	std::cout << "size:" << sizeTriangulos << "\n";
}

void buildCube(int units, int grid, char * filename) {

	std::string dir = path.substr(0, path.length() - 13);
	dir += "models\\";
	dir += filename;
	std::cout << dir << "\n";


	ofstream file;
	file.open(dir); 
	if (!file) { 
		std::cout << "Error opening file" << "\n";
		exit(1);
	}

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
			//std::cout << PointtoString(p1) << "\n";
			//std::cout << PointtoString(p2) << "\n";
			//std::cout << PointtoString(p3) << "\n";
			//std::cout << PointtoString(p4) << "\n";



			//pontos de um quadrado do plano de frente
			p5 = new Point(px1, py1, pz2);
			p7 = new Point(px2, py1, pz2);
			p6 = new Point(px1, py2, pz2); //ponto de cima 
			p8 = new Point(px2, py2, pz2); //ponto de cima  
			//std::cout << PointtoString(p5) << "\n";
			//std::cout << PointtoString(p6) << "\n";
			//std::cout << PointtoString(p7) << "\n";
			//std::cout << PointtoString(p8) << "\n";


			//triangulos do plano de tras
			t1 = new Triangle(p3, p1, p2);//triangulo inferior
			t2 = new Triangle(p3, p2, p4);//trinangulo superior

			//triangulo do plano de frente
			t3 = new Triangle(p5, p7, p8);
			t4 = new Triangle(p5, p8, p6);
			//std::cout << TriangletoString(t1) << "\n";
			//std::cout << TriangletoString(t2) << "\n";
			//std::cout << TriangletoString(t3) << "\n";
			//std::cout << TriangletoString(t4) << "\n";

			triangulos.push_back(t3);
			triangulos.push_back(t4);
			triangulos.push_back(t1);
			triangulos.push_back(t2);
			sizeTriangulos += 4;

			file << TriangleToString(t3) << endl;
			file << TriangleToString(t4) << endl;
			file << TriangleToString(t1) << endl;
			file << TriangleToString(t2) << endl;

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

			file << TriangleToString(t3) << endl;
			file << TriangleToString(t4) << endl;
			file << TriangleToString(t1) << endl;
			file << TriangleToString(t2) << endl;

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

			file << TriangleToString(t3) << endl;
			file << TriangleToString(t4) << endl;
			file << TriangleToString(t1) << endl;
			file << TriangleToString(t2) << endl;
		}
	}
	file.close();
	std::cout << "size:" << sizeTriangulos << "\n";

	for (Triangle t : triangulos) {
		std::cout << std::to_string(t.p1.x) + ";" + std::to_string(t.p1.y) + ";" + std::to_string(t.p1.z) << ",";
		std::cout << std::to_string(t.p2.x) + ";" + std::to_string(t.p2.y) + ";" + std::to_string(t.p2.z) << ",";
		std::cout << std::to_string(t.p3.x) + ";" + std::to_string(t.p3.y) + ";" + std::to_string(t.p3.z) << "\n";
	}
	std::cout << "size:" << sizeTriangulos << "\n";

}


std::string PointToString(Point p) {
	return "("+std::to_string(p.x) + "," + std::to_string(p.y) + "," + std::to_string(p.z) + ")";
}


std::string TriangleToString(Triangle* t) {
	return "p1:" + PointToString(t->p1) + ";" + "p2:" + PointToString(t->p2) + ";" + "p3:" + PointToString(t->p3);
}
