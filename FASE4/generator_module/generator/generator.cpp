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
		buildSphere(radius, slices, stacks, filename);
	}
	else if (strcmp(argv[1], "cone") == 0) {
		float radius = atof(argv[2]);
		int height = atoi(argv[3]);
		int slices = atoi(argv[4]);
		int stacks = atoi(argv[5]);
		char* filename = argv[6];
		buildCone(radius, height, slices, stacks, filename);
	}
	else if (strcmp(argv[1], "cylinder") == 0) {
		float rad = atof(argv[2]);
		int height = atoi(argv[3]);
		int slices = atoi(argv[4]);
		int stacks = atoi(argv[5]);
		char* filename = argv[6];
		buildCylinder(rad, rad, height, slices, stacks, filename);
	}
	else if (strcmp(argv[1], "prisma") == 0) {
		float b_rad = atof(argv[2]);
		float t_rad = atof(argv[3]);
		int height = atoi(argv[4]);
		int slices = atoi(argv[5]);
		int stacks = atoi(argv[6]);
		char* filename = argv[7];
		buildCylinder(b_rad, t_rad, height, slices, stacks, filename);
	}
	else if (strcmp(argv[1], "torus") == 0) {
		float rad1 = atof(argv[2]);
		float rad2 = atof(argv[3]);
		int slices = atoi(argv[4]);
		int stacks = atoi(argv[5]);
		char* filename = argv[6];
		buildTorus(rad1, rad2, slices, stacks, filename);
	}
	else if (strcmp(argv[1], "patch") == 0) {
		char* fpatch = argv[2];
		int tesLvl = atoi(argv[3]);
		char* filename = argv[4];
		buildTeapot(fpatch, tesLvl, filename);
	}
	else {
		std::cout << argv[0] << "\n";
	}
}

void buildPlane(int units, int divs, char* filename) {
	// Open the output file
	std::string dir = path;
	dir += "\\models\\";
	dir += filename;

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
	fs::create_directory("models");
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

void buildCube(int units, int grid, char* filename) {
	// Open the output file
	std::string dir = path;
	dir += "\\models\\";
	dir += filename;

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
	fs::create_directory("models");
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
	// Open the output file
	std::string dir = path;
	dir += "\\models\\";
	dir += filename;

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
	fs::create_directory("models");
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
	std::string dir = path;
	dir += "\\models\\";
	dir += filename;

	float step = (M_PI * 2) / slices;
	float stackStep = static_cast<float>(height) / static_cast<float>(stacks);
	float radStep = static_cast<float>(radius) / static_cast<float>(stacks);
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
	float curHeight;
	float curRad;

	for (int i = 0; i < slices; i++) {
		curHeight = 0.0f;
		curRad = radius;

		for (int j = 0; j < stacks - 1; j++) {
			p1 = new Point(curRad * sin(alpha), curHeight, curRad * cos(alpha));
			p2 = new Point(curRad * sin(alpha + step), curHeight, curRad * cos(alpha + step));
			p3 = new Point((curRad - radStep) * sin(alpha + step), curHeight + stackStep, (curRad - radStep) * cos(alpha + step));
			p4 = new Point((curRad - radStep) * sin(alpha), curHeight + stackStep, (curRad - radStep) * cos(alpha));

			triangulos.push_back(new Triangle(p1, p2, p3));
			triangulos.push_back(new Triangle(p3, p4, p1));

			curHeight += stackStep;
			curRad -= radStep;
		}

		alpha += step;
	}

	alpha = 0.0f;

	for (int c = 0; c < slices; c++) {
		p1 = new Point(curRad * sin(alpha + step), curHeight, curRad * cos(alpha + step));
		p2 = new Point(curRad * sin(alpha), curHeight, curRad * cos(alpha));
		p3 = new Point(0.0f, height, 0.0f);

		triangulos.push_back(new Triangle(p3, p2, p1));

		alpha += step;
	}

	ofstream file;
	fs::create_directory("models");
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

void buildCylinder(float b_rad, float t_rad, int height, int slices, int stacks, const char* filename) {
	// Open the output file
	std::string dir = path;
	dir += "\\models\\";
	dir += filename;

	float step = (M_PI * 2) / slices;
	float stackStep = static_cast<float>(height) / static_cast<float>(stacks);
	float radStep = abs(b_rad - t_rad) / stacks;
	float alpha = 0.0f;
	float h_height = static_cast<float>(height) / 2;
	Point* p1, * p2, * p3, * p4;

	for (int c = 0; c < slices; c++) {
		//Lower circle
		p1 = new Point(b_rad * sin(alpha + step), -h_height, b_rad * cos(alpha + step));
		p2 = new Point(b_rad * sin(alpha), -h_height, b_rad * cos(alpha));
		p3 = new Point(0.0f, -h_height, 0.0f);

		triangulos.push_back(new Triangle(p1, p2, p3));

		//Upper circle
		p1 = new Point(t_rad * sin(alpha + step), h_height, t_rad * cos(alpha + step));
		p2 = new Point(t_rad * sin(alpha), h_height, t_rad * cos(alpha));
		p3 = new Point(0.0f, h_height, 0.0f);

		triangulos.push_back(new Triangle(p3, p2, p1));

		alpha += step;
	}

	alpha = 0.0f;
	float curHeight;
	float curRad;

	if (t_rad > b_rad)
		for (int i = 0; i < slices; i++) {
			curHeight = static_cast<float>(height) / 2;

			curRad = t_rad;

			for (int j = 0; j < stacks; j++) {
				p1 = new Point(curRad * sin(alpha), curHeight, curRad * cos(alpha));
				p2 = new Point(curRad * sin(alpha + step), curHeight, curRad * cos(alpha + step));
				p3 = new Point((curRad - radStep) * sin(alpha + step), curHeight - stackStep, (curRad - radStep) * cos(alpha + step));
				p4 = new Point((curRad - radStep) * sin(alpha), curHeight - stackStep, (curRad - radStep) * cos(alpha));

				triangulos.push_back(new Triangle(p3, p2, p1));
				triangulos.push_back(new Triangle(p1, p4, p3));

				curHeight -= stackStep;
				curRad -= radStep;
			}

			alpha += step;
		}
	else
		for (int i = 0; i < slices; i++) {
			curHeight = -(static_cast<float>(height) / 2);

			curRad = b_rad;

			for (int j = 0; j < stacks; j++) {
				p1 = new Point(curRad * sin(alpha), curHeight, curRad * cos(alpha));
				p2 = new Point(curRad * sin(alpha + step), curHeight, curRad * cos(alpha + step));
				p3 = new Point((curRad - radStep) * sin(alpha + step), curHeight + stackStep, (curRad - radStep) * cos(alpha + step));
				p4 = new Point((curRad - radStep) * sin(alpha), curHeight + stackStep, (curRad - radStep) * cos(alpha));

				triangulos.push_back(new Triangle(p1, p2, p3));
				triangulos.push_back(new Triangle(p3, p4, p1));

				curHeight += stackStep;
				curRad -= radStep;
			}

			alpha += step;
		}

	ofstream file;
	fs::create_directory("models");
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

void buildTorus(float rad1, float rad2, int slices, int stacks, char* filename) {
	// Open the output file
	std::string dir = path;
	dir += "\\models\\";
	dir += filename;

	float deltaAlpha = (M_PI * 2) / slices;
	float deltaBeta = (M_PI * 2) / stacks;
	float alpha = 0.0f;
	float beta = 0.0f;
	Point* p1, * p2, * p3, * p4;

	for (int i = 0; i < slices; i++) {
		alpha = i * deltaAlpha;

		for (int j = 0; j < stacks; j++) {
			beta = j * deltaBeta;

			p1 = new Point((rad1 + rad2 * cos(beta)) * cos(alpha),
				rad2 * sin(beta),
				(rad1 + rad2 * cos(beta)) * sin(alpha));
			p2 = new Point((rad1 + rad2 * cos(beta)) * cos(alpha + deltaAlpha), 
				rad2 * sin(beta),
				(rad1 + rad2 * cos(beta)) * sin(alpha + deltaAlpha));
			p3 = new Point((rad1 + rad2 * cos(beta + deltaBeta)) * cos(alpha + deltaAlpha),
				rad2 * sin(beta + deltaBeta),
				(rad1 + rad2 * cos(beta + deltaBeta)) * sin(alpha + deltaAlpha));
			p4 = new Point((rad1 + rad2 * cos(beta + deltaBeta)) * cos(alpha), 
				rad2 * sin(beta + deltaBeta),
				(rad1 + rad2 * cos(beta + deltaBeta)) * sin(alpha));

			triangulos.push_back(new Triangle(p3, p2, p1));
			triangulos.push_back(new Triangle(p1, p4, p3));
		}
	}

	ofstream file;
	fs::create_directory("models");
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

void buildTeapot(char* fpatch, int tesLvl, char* filename) {
	ifstream pfile;
	pfile.open(fpatch);
	if (pfile.is_open() != true) {
		printf("Failed to open Patch File!\n");
		_exit(1);
	}

	int patchNum, vertexNum;
	string line, seg;

	getline(pfile, line);
	patchNum = stoi(line);

	int *patch_index = (int*)malloc(patchNum * 16 * sizeof(int));

	for (int i = 0; i < patchNum; i++) {
		getline(pfile, line);
		stringstream tmpss(line);

		for (int c = 0; c < 16; c++) {
			getline(tmpss, seg, ',');
			patch_index[i * 16 + c] = stoi(seg);
		}
	}

	getline(pfile, line);
	vertexNum = stoi(line);

	float* vertex = (float*)malloc(vertexNum * 3 * sizeof(float));

	for (int i = 0; i < vertexNum; i++) {
		getline(pfile, line);
		stringstream tmpss(line);

		for (int c = 0; c < 3; c++) {
			getline(tmpss, seg, ',');
			vertex[i * 3 + c] = stof(seg);
		}
	}

	pfile.close();

	float m[4][4] = { {-1.0f, 3.0f, -3.0f, 1.0f},
					{3.0f, -6.0f, 3.0f, 0.0f},
					{-3.0f, 3.0f, 0.0f, 0.0f},
					{1.0f, 0.0f, 0.0f, 0.0f} };

	float step = 1.0f / tesLvl;
	Point p1, p2, p3, p4;
	vector<Point> points;

	for(int i = 0; i < patchNum; i++) {
		Point p[4][4];

		for (int l = 0; l < 4; l++)
			for (int c = 0; c < 4; c++) {
				int index = patch_index[i * 16 + l * 4 + c];
				p[l][c] = new Point(vertex[3 * index + 0], vertex[3 * index + 1], vertex[3 * index + 2]);
			}

		Point res[4][4];
		calcAMat(m[0], p[0], res[0]);

		for (int j = 0; j < tesLvl; j++) {
			float u = step * j;
			float u_vec[4] = { powf(u, 3.0f), powf(u, 2.0f), u, 1.0f };

			u = step * (j + 1);
			float u_vec2[4] = { powf(u, 3.0f), powf(u, 2.0f), u, 1.0f };

			Point res1[4], res2[4];

			multMatrixVector(res[0], u_vec, res1);
			multMatrixVector(res[0], u_vec2, res2);

			for (int c = 0; c < tesLvl; c++) {
				float v = step * c;
				float v_vec[4] = { powf(v, 3.0f), powf(v, 2.0f), v, 1.0f };

				p1 = multVects(res1, v_vec);
				p2 = multVects(res2, v_vec);

				v = step * (c + 1);
				float v_vec2[4] = { powf(v, 3.0f), powf(v, 2.0f), v, 1.0f };

				p3 = multVects(res1, v_vec2);
				p4 = multVects(res2, v_vec2);

				points.push_back(p2);
				points.push_back(p4);
				points.push_back(p3);

				points.push_back(p3);
				points.push_back(p1);
				points.push_back(p2);
			}
		}
	}

	ofstream file;
	file.open(filename);
	if (file.is_open() != true) {
		printf("Failed to open output file!\n");
		_exit(1);
	}

	file << to_string(points.size()) << "\n";
	std::cout << "Writing: <" << to_string(points.size()) << "> Points!" << endl;

	for (int c = 0; c < points.size(); c++)
		file << PointToString(points[c]) << "\n";

	file.close();
	std::free(patch_index);
	std::free(vertex);
}

void multMatrixVector(Point* m, float *vec, Point* res) {
	for (int j = 0; j < 4; j++) {
		res[j] = new Point();

		for (int k = 0; k < 4; k++)
			res[j].matOp(vec[k], m[j * 4 + k]);
	}
}

void calcAMat(float* m, Point* points, Point* res) {
	Point tmp[4][4];
	for (int i = 0; i < 4; i++) 
		for (int j = 0; j < 4; j++) {
			tmp[i][j] = new Point();

			for (int k = 0; k < 4; k++)
				tmp[i][j].matOp(m[i * 4 + k], points[k * 4 + j]);
		}

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			res[i * 4 + j] = new Point();

			for (int k = 0; k < 4; k++)
				res[i * 4 + j].matOp(m[k * 4 + j], tmp[i][k]);
		}
}

Point multVects(Point u[4], float v[4]) {
	Point res = new Point();

	for (int i = 0; i < 4; i++)
		res.matOp(v[i], u[i]);

	return res;
}

void cross(float* a, float* b, float* res) {
	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(float* a) {
	float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0] / l; a[1] = a[1] / l; a[2] = a[2] / l;
}

std::string PointToString(Point p) {
	return to_string(p.x) + "," + to_string(p.y) + "," + to_string(p.z);
}


std::string TriangleToString(Triangle* t) {
	return PointToString(t->p1) + "\n" + PointToString(t->p2) + "\n" + PointToString(t->p3) + "\n";
}