#include "tinyxml2.h"
#include "engine.h"


#define M_FIX 0
#define M_FPS 1
#define STEP ((M_PI*2) / 20)
#define VBO_OFF 0
#define VBO_BASIC 1
#define VBO_INDEX 2
using namespace tinyxml2;

int sizeTriangulos = 0;
int mode = M_FIX;
int vbo_mode = VBO_BASIC;

vector<string> allmodels;
vector<Group> my_world;

float width = 0, height = 0;
float posCamx = 0, posCamy = 0, posCamz = 0;
float lookCamx = 0, lookCamy = 0, lookCamz = 0;
float upCamx = 0, upCamy = 0, upCamz = 0;
float fov = 0, near = 0, far = 0;
float G_alpha = 0.0f;
float G_beta = 0.0f;
float G_radious = 5.0f;

double frames;
int timebase;
float gt = 0;

int groupCount = 0;
GLuint* buffers;


void framerate() {
	char title[50];
	frames++;
	double time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000) {
		double fps = frames * 1000.0 / (time - timebase);
		timebase = time;
		frames = 0;
		sprintf(title, "CG@DI-TP | %lf FPS", fps);
		glutSetWindowTitle(title);
	}
}

void buildRotMatrix(float* x, float* y, float* z, float* m) {

	m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
	m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
	m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}


void cross(float* a, float* b, float* res) {

	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}


void normalize(float* a) {

	float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0] / l;
	a[1] = a[1] / l;
	a[2] = a[2] / l;
}

void multMatrixVector(float* m, float* v, float* res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}
}

void getCatmullRomPoint(float t, float* p0, float* p1, float* p2, float* p3, float* pos, float* deriv) {

	// catmull-rom matrix
	float m[4][4] = { {-0.5f,  1.5f, -1.5f,  0.5f},
						{ 1.0f, -2.5f,  2.0f, -0.5f},
						{-0.5f,  0.0f,  0.5f,  0.0f},
						{ 0.0f,  1.0f,  0.0f,  0.0f} };
	float t3 = powf(t, 3.0f);
	float t2 = powf(t, 2.0f);
	float T[4] = { t3, t2, t, 1.0f };
	float Td[4] = { 3 * t2, 2 * t, 1.0f, 0.0f };

	for (int i = 0; i < 3; i++) {
		float A[4];
		float vec[4] = { p0[i], p1[i], p2[i], p3[i] };
		multMatrixVector(&m[0][0], vec, A);

		pos[i] = 0;
		deriv[i] = 0;

		for (int j = 0; j < 4; j++) {
			pos[i] += T[j] * A[j];
			deriv[i] += Td[j] * A[j];
		}
	}
}

void getGlobalCatmullRomPoint(float gt, float* pos, float* deriv, vector<float> curvepoints, int size) {

	float t = gt * (size / 3); // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

	// indices store the points
	int indices[4];
	indices[0] = (index + (size / 3) - 1) % (size / 3);
	indices[1] = (indices[0] + 1) % (size / 3);
	indices[2] = (indices[1] + 1) % (size / 3);
	indices[3] = (indices[2] + 1) % (size / 3);
	int a = 1;

	float p[4][3];
	for (int i = 0; i < (size/3) ; i++) {
		int j = i * 3;
		p[i][0] = curvepoints[j];
		p[i][1] = curvepoints[j+1];
		p[i][2] = curvepoints[j+2];
	}

	getCatmullRomPoint(t, p[indices[0]], p[indices[1]], p[indices[2]], p[indices[3]], pos, deriv);
}

void renderCatmullRomCurve(vector<float> curvepoints) {
	// draw curve using line segments with GL_LINE_LOOP
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; ++i) {
		float pp[3];
		float dv[3];
		float r = i / 100.0;
		getGlobalCatmullRomPoint(r, pp, dv, curvepoints, curvepoints.size());
		glVertex3f(pp[0], pp[1], pp[2]);
	}
	glEnd();

}


void changeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you can�t make a window with zero width).
	if (h == 0)
		h = 1;
	// compute window's aspect ratio
	float ratio = w * 1.0f / h;
	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load the identity matrix
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// Set the perspective
	gluPerspective(fov, ratio, near, far);
	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void renderScene(void)
{
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set camera
	glLoadIdentity();
	gluLookAt(posCamx, posCamy, posCamz,
		lookCamx, lookCamy, lookCamz,
		upCamx, upCamy, upCamz);
	glColor3f(1.0f, 1.0f, 1.0f);

	// Axis
	glBegin(GL_LINES);
	// X axis Red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);
	//Y Axis Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
	//Z axis Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();

	build_groups(my_world);

	framerate();

	// End of frame
	glutSwapBuffers();
}

void alignment(float* deriv) {
	float yi[3] = { 0,1,0 };
	float x[3] = { deriv[0], deriv[1], deriv[2] };
	normalize(x);
	float z[3];
	cross(x, yi, z);
	normalize(z);
	float y[3];
	cross(z, x, y);
	normalize(y);

	float matriz[16];
	buildRotMatrix(x, y, z, matriz);

	glMultMatrixf(matriz);
}

void prepareData_BasicVBO(vector<Group> groups) {
	for (Group g : groups) {
		glBindBuffer(GL_ARRAY_BUFFER, buffers[g.getBufIndex()]);
		glBufferData(GL_ARRAY_BUFFER,
			sizeof(float) * g.getModelos().size(),
			g.getModelos().data(),
			GL_STATIC_DRAW);

		//Children
		if (g.getChild().size() > 0)
			prepareData_BasicVBO(g.getChild());
	}
}

void build_groups(vector<Group> groups) {
	for (Group g : groups) {
		// Transformations
		glPushMatrix();

		for (Transformation tra : g.getTransformations()) {
			if (tra.transformation_name.compare("translate") == 0) {
				if (tra.curvepoints.size() >= 12) {
					//construcao das Catmull-Rom cubic curves requer a existencia de pelo menos 4 pontos

					float pos[3], deriv[3];
					static float t = 0;
					float deltat = 0.0f;
					if (tra.time != 0) {
						deltat = 1.0f / (tra.time * 1000) ;
					}

					renderCatmullRomCurve(tra.curvepoints);

					getGlobalCatmullRomPoint(t, pos, deriv, tra.curvepoints, tra.curvepoints.size());
					glTranslatef(pos[0], pos[1], pos[2]);

					if (tra.align) {
						alignment(deriv);
					}

					if (tra.time != 0) t += deltat;
				}
				else {
					glTranslatef(tra.trsx, tra.trsy, tra.trsz);
				}
			}
			else if (tra.transformation_name.compare("rotate") == 0) {
				// angle can be replaced with time, test now which one
				if (tra.time == -1) {
					glRotatef(tra.angle, tra.trsx, tra.trsy, tra.trsz);
				}
				else {
					//number of seconds to perform a full 360 degrees rotation
					int timesys = glutGet(GLUT_ELAPSED_TIME);
					float angulo = (timesys / 1000.0) / (int)tra.time * 360;
					glRotatef(angulo, tra.trsx, tra.trsy, tra.trsz);
				}
			}
			else if (tra.transformation_name.compare("scale") == 0)
				glScalef(tra.trsx, tra.trsy, tra.trsz);
		}

		// Models
		glColor3f(1.0f, 0.5f, 0.0f);
		if (vbo_mode == VBO_OFF)
			for (int c = 0; c < g.modelos.size();c += 9) {
				glBegin(GL_TRIANGLES);
					glVertex3f(g.modelos[c], g.modelos[c + 1], g.modelos[c + 2]);
					glVertex3f(g.modelos[c + 3], g.modelos[c + 4], g.modelos[c + 5]);
					glVertex3f(g.modelos[c + 6], g.modelos[c + 7], g.modelos[c + 8]);
				glEnd();
			}
		else if (vbo_mode == VBO_BASIC) {
			glBindBuffer(GL_ARRAY_BUFFER, buffers[g.getBufIndex()]);
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glDrawArrays(GL_TRIANGLES, 0, g.modelos.size() / 3);
		}

		//Children
		if (g.getChild().size() > 0)
			build_groups(g.getChild());

		glPopMatrix();
	}
}

void processKeys(unsigned char c, int xx, int yy) {
	// put code to process regular keys in here
	if (mode == M_FIX) {
		if (c == 'a' || c == 'A')
			G_alpha -= STEP;
		else if (c == 'd' || c == 'D') {
			G_alpha += STEP;
		}
		else if ((c == 'w' || c == 'W') && G_beta < 1.5f) {
			G_beta += STEP;
		}
		else if ((c == 's' || c == 'S') && G_beta > -1.5f) {
			G_beta -= STEP;
		}
		else if ((c == 'q' || c == 'Q') && G_radious > 0.5f) {
			G_radious -= 0.5f;
		}
		else if (c == 'e' || c == 'E') {
			G_radious += 0.5f;
		}
	}
	else {
		if (c == 'a' || c == 'A') {

		}
		else if (c == 'd' || c == 'D') {

		}
		else if (c == 'w' || c == 'W') {

		}
		else if (c == 's' || c == 'S') {

		}
		else if (c == 'q' || c == 'Q') {

		}
		else if (c == 'e' || c == 'E') {

		}
	}

	posCamx = G_radious * cos(G_beta) * sin(G_alpha);
	posCamy = G_radious * sin(G_beta);
	posCamz = G_radious * cos(G_beta) * cos(G_alpha);

	if (c == 'v' || c == 'V') {
		vbo_mode += 1;

		if (vbo_mode > 1) //TODO CHANGE AFTER VBO INDEX IMPLEMENTED
			vbo_mode = 0;

		switch (vbo_mode)
		{
		case 0:
			printf("VBO OFF\n");
			break;
		case 1:
			printf("VBO BASIC\n");
			break;
		case 2:
			printf("VBO INDEX\n");
			break;
		default:
			break;
		}
	}

	glutPostRedisplay();
}


void processSpecialKeys(int key, int xx, int yy) {
	// put code to process special keys in here
	if (key == GLUT_KEY_END) {
		glPolygonMode(GL_FRONT, GL_FILL);
	}
	else if (key == GLUT_KEY_PAGE_UP) {
		glPolygonMode(GL_FRONT, GL_LINE);
	}
	else if (key == GLUT_KEY_PAGE_DOWN) {
		glPolygonMode(GL_FRONT, GL_POINT);
	}

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	std::string xmlfile;
	if (argc == 2) {
		xmlfile = argv[1];
	}
	else xmlfile = "config.xml";

	parse_XML(xmlfile);
	// put GLUT�s init here
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("CG-TP");

	// put callback registry here
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutDisplayFunc(renderScene);

	// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

	//VBO Stuff Inits
#ifndef __APPLE__
	glewInit();
#endif

	glEnableClientState(GL_VERTEX_ARRAY);

	//VBO Preping
	buffers = (GLuint*)calloc(groupCount, sizeof(GLuint));

	if (!buffers) {
		printf("Failed to allocate buffer memory!\nNot Using VBOs!\n");
		vbo_mode = VBO_OFF;
	}

	glGenBuffers(groupCount, buffers);
	prepareData_BasicVBO(my_world);

	// some OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glPolygonMode(GL_FRONT, GL_LINE);

	timebase = glutGet(GLUT_ELAPSED_TIME);

	// enter GLUT�s main cycle
	glutMainLoop();

	return 1;
}

void parse_XML(std::string xmlfile) {
	XMLDocument document;
	std::string path = fs::current_path().string();
	string dir = path + "\\";
	dir += xmlfile;

	char* pathfile = new char[dir.length() + 1];
	std::strcpy(pathfile, dir.c_str());

	cout << path << endl;

	cout << pathfile << endl;

	bool loadOkay = document.LoadFile(pathfile);
	if (loadOkay == 0) {
		XMLElement* world = document.FirstChildElement("world");
		XMLElement* window = world->FirstChildElement("window");
		//define window size
		if (window != nullptr) {
			width = stof(window->Attribute("width"));
			height = stof(window->Attribute("height"));
		}
		else {
			width = 512.0f;
			height = 512.0f;
		}

		XMLElement* camera = window->NextSiblingElement("camera");

		XMLElement* position = camera->FirstChildElement("position");
		//define camera position
		posCamx = stof(position->Attribute("x"));
		posCamy = stof(position->Attribute("y"));
		posCamz = stof(position->Attribute("z"));

		XMLElement* lookat = position->NextSiblingElement("lookAt");
		//define camera view
		lookCamx = stof(lookat->Attribute("x"));
		lookCamy = stof(lookat->Attribute("y"));
		lookCamz = stof(lookat->Attribute("z"));

		XMLElement* up = camera->FirstChildElement("up");
		//define camera vector
		if (up != nullptr) {
			upCamx = stof(up->Attribute("x"));
			upCamy = stof(up->Attribute("y"));
			upCamz = stof(up->Attribute("z"));
		}
		else {
			upCamx = 0.0f;
			upCamy = 1.0f;
			upCamz = 0.0f;
		}

		XMLElement* projection = camera->FirstChildElement("projection");
		//define camera projection
		if (projection != nullptr) {
			fov = stof(projection->Attribute("fov"));
			near = stof(projection->Attribute("near"));
			far = stof(projection->Attribute("far"));
		}
		else {
			fov = 60.0f;
			near = 1.0f;
			far = 1000.0f;
		}

		//calculate camera starting angle and radius
		G_radious = sqrt(pow(posCamx, 2.0f) + pow(posCamy, 2.0f) + pow(posCamz, 2.0f));
		G_beta = atan(posCamy / G_radious);
		G_alpha = acos(posCamz / (sqrt(pow(posCamx, 2.0f) + pow(posCamz, 2.0f))));

		XMLElement* grupo = camera->NextSiblingElement("group");

		while (grupo != nullptr) {
			getGroups(grupo, true);
			grupo = grupo->NextSiblingElement("group");
		}
		vector<Group> v = my_world;
		cout << "Tamanho:" << endl;
		cout << my_world.size() << endl;

		cout << "height= " << height << endl;
		cout << "width = " << width << endl;
		cout << "pos x= " << posCamx << endl;
		cout << "pos y = " << posCamy << endl;
		cout << "pos z = " << posCamz << endl;
		cout << "look x= " << lookCamx << endl;
		cout << "look y = " << lookCamy << endl;
		cout << "look z = " << lookCamz << endl;
		cout << "up x= " << upCamx << endl;
		cout << "up y = " << upCamy << endl;
		cout << "up z = " << upCamz << endl;
		cout << "fov = " << fov << endl;
		cout << "near = " << near << endl;
		cout << "far = " << far << endl;
	}
	else {
		cout << "Failed to load XML file" << endl;
	}
}

vector<float> getModels(vector<string> ms) {
	vector<float> vertex;

	for (string fname : ms) {

		std::ifstream myfile;
		string path = fs::current_path().string();
		string dirpath = path + "\\";
		dirpath += fname;
		myfile.open(dirpath);
		string line, seg;
		int vertex_num = 0;

		if (myfile.is_open()) {
			cout << "Reading from: " << fname << endl;

			getline(myfile, seg);

			vertex_num = stoi(seg);

			vector<float> tmp;

			for (int i = 0; i < vertex_num; i++) {
				getline(myfile, line);
				stringstream tmpss(line);

				for (int c = 0; c < 3; c++) {
					getline(tmpss, seg, ',');
					tmp.push_back(stof(seg));
				}

				vertex.push_back(tmp[0]);
				vertex.push_back(tmp[1]);
				vertex.push_back(tmp[2]);
				tmp.clear();
			}
		}
		else {
			cout << "Failed to open " << fname << endl;
		}
	}

	cout << "Primitivas" << "=>";
	cout << ms.size() << endl;

	return vertex;
}

Group getGroups(XMLElement* xmlelement, bool top_lvl) {
	float x = 0;
	float y = 0;
	float z = 0;
	float bx = 0;
	float by = 0;
	float bz = 0;
	float angle = 0;
	float time = -1;
	bool align = false;
	vector<Transformation> transformacoes;
	vector<string> modelos;
	vector<float> curvepoints;

	XMLElement* transformacao = xmlelement->FirstChildElement("transform");

	if (transformacao != nullptr) {
		for (XMLElement* tras = transformacao->FirstChildElement(); tras != nullptr; tras = tras->NextSiblingElement()) {

			if (tras != nullptr && tras->Attribute("time") == nullptr) {

				if (tras->Attribute("x") != nullptr) {
					x = stof(tras->Attribute("x"));
				}
				if (tras->Attribute("y") != nullptr) {
					y = stof(tras->Attribute("y"));
				}
				if (tras->Attribute("z") != nullptr) {
					z = stof(tras->Attribute("z"));
				}
				if (tras->Attribute("angle") != nullptr) {
					angle = stof(tras->Attribute("angle"));
				}
				Transformation t = new Transformation(tras->Name(), x, y, z, time, align);

				if (angle != 0)
					t.setAngle(angle);
				angle = 0;
				time = -1;
				transformacoes.push_back(t);
			}

			if (tras != nullptr && tras->Attribute("time") != nullptr) {
				if (tras->Attribute("time") != nullptr) {
					time = stof(tras->Attribute("time"));
				}
				if (tras->Attribute("align") != nullptr) {
					if (strcmp(tras->Attribute("align"), "true") == 0)
						align = true;
					else align = false;
				}
				if (tras->Attribute("x") != nullptr) {
					x = stof(tras->Attribute("x"));
				}
				if (tras->Attribute("y") != nullptr) {
					y = stof(tras->Attribute("y"));
				}
				if (tras->Attribute("z") != nullptr) {
					z = stof(tras->Attribute("z"));
				}

				if (strcmp(tras->Name(), "translate") == 0) {
					for (XMLElement* ponto = tras->FirstChildElement(); ponto != nullptr; ponto = ponto->NextSiblingElement()) {
						if (ponto->Attribute("x") != nullptr) {
							bx = stof(ponto->Attribute("x"));
							curvepoints.push_back(bx);
						}
						if (ponto->Attribute("y") != nullptr) {
							by = stof(ponto->Attribute("y"));
							curvepoints.push_back(by);
						}
						if (ponto->Attribute("z") != nullptr) {
							bz = stof(ponto->Attribute("z"));
							curvepoints.push_back(bz);
						}
					}

					Transformation t = new Transformation(tras->Name(), x, y, z, time, align);
					t.setTime(time);
					t.setAlign(align);
					for (int p = 0; p < curvepoints.size(); p++) {
						t.addPoint(curvepoints[p]);
					}
					transformacoes.push_back(t);
					time = -1;
					curvepoints.clear();
				}
				else {
					Transformation t = new Transformation(tras->Name(), x, y, z, time, align);
					t.setTime(time);
					t.setAlign(align);
					time = -1;
					transformacoes.push_back(t);
				}
			}
		}
	}

	XMLElement* models = xmlelement->FirstChildElement("models");
	if (models != nullptr) {
		XMLElement* model = models->FirstChildElement("model");

		while (model != nullptr) {

			if (model->Attribute("file") != nullptr) {
				allmodels.push_back(model->Attribute("file"));
				modelos.push_back(model->Attribute("file"));
			}
			model = model->NextSiblingElement();
		}
	}
	vector <float> pontos = getModels(modelos);
	Group groupElement = Group(transformacoes);
	int pp = 0;
	while (pp != pontos.size()) {
		groupElement.addPointModel(pontos[pp]);
		pp += 1;
	}
	groupElement.setBufIndex(groupCount++); // Needed to know which VBO buffer to use for group

	for (tinyxml2::XMLElement* child = xmlelement->FirstChildElement("group");
		child != NULL; child = child->NextSiblingElement("group"))
	{
		Group chld = getGroups(child, false);
		//get the nested children group
		groupElement.addChild(chld);
	}

	cout << "modelos:" + getModelos(modelos) << endl;

	if (top_lvl == true)
		my_world.push_back(groupElement);

	return groupElement;
}

string TransformationToString(Transformation t) {
	if (t.transformation_name == "rotation") {
		return t.transformation_name + ";" + to_string(t.trsx) + ";" + to_string(t.trsy) + ";" + to_string(t.trsz) + ";" + to_string(t.angle) + ";";
	}
	else {
		return t.transformation_name + ";" + to_string(t.trsx) + ";" + to_string(t.trsy) + ";" + to_string(t.trsz) + ";";
	}
}

string getTransformacoes(vector<Transformation> trans) {
	string trasformacoes = "";
	int k = 0;
	while (k < trans.size()) {
		trasformacoes += TransformationToString(trans[k]);
		k += 1;
	}
	return trasformacoes;
}

string getModelos(vector<string> modelos) {
	string models = "";
	int k = 0;
	while (k < modelos.size()) {
		models += modelos[k];
		k += 1;
	}
	return models;
}
