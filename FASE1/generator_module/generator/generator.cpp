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
		int radius = atoi(argv[2]);
		int slices = atoi(argv[3]);
		int stacks = atoi(argv[4]);
		char* filename = argv[5];
		buildSphere(radius,slices,stacks,filename);
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

	//valores das coordenadas dos pontos do triangulos do plano da frente e de tr�s
	float px1, py1, pz1;
	float px2, py2, pz2;
	Point* p1, * p2, * p3, * p4, * p5, * p6, * p7, * p8;
	Triangle* t1, * t2, * t3, * t4;
	//para z constante
	//vamos desenhar os dois planos paralelos que s�o perpendiculares ao eixo OZ
	for (int i = 0; -finalx + i * stepx < finalx; i++) {
		for (int j = 0; -finaly + j * stepy < finaly; j++) {

			px1 = -finalx + (i * stepx);
			px2 = -finalx + ((i + 1) * stepx);
			py1 = -finaly + (j * stepy);
			py2 = -finaly + ((j + 1) * stepy);
			//plano de tr�s
			pz1 = -finalz;
			//plano de frente
			pz2 = finalz;

			//pontos de um quadrado do plano de tr�s
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
			//plano de tr�s
			px1 = -finalx;
			//plano de frente
			px2 = finalx;

			//pontos de um quadrado do plano de tr�s
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

void buildSphere(float radius, int slices, int stacks, const char* filename)
{
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
	Point* p1, * p2, * p3, * p4;
	Triangle* t1, * t2;
	double pi = M_PI;

	float deltaPhi = pi / stacks;
	float deltaTheta = 2 * pi / slices;

	for (int i = 0; i < stacks; i++) {
		float phi = i * deltaPhi;
		float nextPhi = (i + 1) * deltaPhi;
		for (int j = 0; j < slices; j++) {
			float theta = j * deltaTheta;
			float nextTheta = (j + 1) * deltaTheta;

			p1 = new Point (radius * sin(phi) * cos(theta), radius * sin(phi) * sin(theta), radius * cos(phi));
			p2 = new Point(radius * sin(phi) * cos(nextTheta), radius * sin(phi) * sin(nextTheta), radius * cos(phi));
			p3 = new Point(radius * sin(nextPhi) * cos(theta), radius * sin(nextPhi) * sin(theta), radius * cos(nextPhi));
			p4 = new Point(radius * sin(nextPhi) * cos(nextTheta), radius * sin(nextPhi) * sin(nextTheta), radius * cos(nextPhi));

			t1 = new Triangle (p1, p2, p4);
			t2 = new Triangle (p1, p4, p3);


			triangulos.push_back(t1);
			triangulos.push_back(t2);
			sizeTriangulos += 2;

			file << TriangleToString(t1) << std::endl;
			file << TriangleToString(t2) << std::endl;
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


void buildCone(float radius, int height, int slices, int stacks, const char* filename)
{
	// Open the output file
	std::string dir = "models\\" + std::string(filename);
	std::ofstream file(dir);
	if (!file.is_open()) {
		std::cout << "Error opening file" << std::endl;
		return;
	}

	// Calculate the angle increments for slices and stacks
	Point* p1;
	const double pi = M_PI;
	const float deltaPhi = pi / stacks;
	const float deltaTheta = 2 * pi / slices;

	// Create the vertices and triangles of the cone
	std::vector<Point> vertices;
	for (int i = 0; i <= stacks; i++) {
		const float phi = i * deltaPhi;
		const float y = height * (float)i / stacks;
		for (int j = 0; j <= slices; j++) {
			const float theta = j * deltaTheta;
			const float x = radius * sin(phi) * cos(theta);
			const float z = radius * sin(phi) * sin(theta);
			p1 = new Point(x, y, z);
			vertices.push_back(p1);
		}
	}
	
	for (int i = 0; i < stacks; i++) {
		for (int j = 0; j < slices; j++) {
			const int p1 = i * (slices + 1) + j;
			const int p2 = p1 + 1;
			const int p3 = (i + 1) * (slices + 1) + j;
			const int p4 = p3 + 1;
			triangulos.push_back(Triangle(&vertices[p1], &vertices[p3], &vertices[p2]));
			triangulos.push_back(Triangle(&vertices[p2], &vertices[p3], &vertices[p4]));
		}
	}

	// Write the triangles to the file
	for (Triangle t : triangulos) {
		file << TriangleToString(&t) << std::endl;
	}

	// Close the file and print the number of triangles
	file.close();
	std::cout << "Cone with " << triangulos.size() << " triangles written to file " << dir << std::endl;
}
