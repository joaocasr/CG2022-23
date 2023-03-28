#include "tinyxml2.h"
#include "engine.h"

#define M_FIX 0
#define M_FPS 1
#define STEP ((M_PI*2) / 20)
using namespace tinyxml2;

int sizeTriangulos = 0;
int mode = M_FIX;

vector<string> allmodels;
vector<Primitiva> primitivas;
vector<Group> my_world;

int testing_var = 0; //TODO REMOVE LATER

float width = 0, height = 0;
float posCamx = 0, posCamy = 0, posCamz = 0;
float lookCamx = 0, lookCamy = 0, lookCamz = 0;
float upCamx = 0, upCamy = 0, upCamz = 0;
float fov = 0, near = 0, far = 0;
float G_alpha = 0.0f;
float G_beta = 0.0f;
float G_radious = 5.0f;

void build_groups(vector<Group> groups);

void changeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you can’t make a window with zero width).
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

	// End of frame
	glutSwapBuffers();
}

void build_groups(vector<Group> groups) {
	for (Group g : groups) {
		// Transformations
		glPushMatrix();

		for (Transformation tra : g.getTransformations()) {
			cout << tra.transformation_name << endl;
			if (tra.transformation_name.compare("translate") == 0)
				glTranslatef(tra.trsx, tra.trsy, tra.trsz);
			else if (tra.transformation_name.compare("rotation") == 0)
				glRotatef(tra.angle, tra.trsx, tra.trsy, tra.trsz);
			else if (tra.transformation_name.compare("scale") == 0)
				glScalef(tra.trsx, tra.trsy, tra.trsz);
		}

		// Models
		glColor3f(1.0f, 0.5f, 0.0f);
		for (int c = 0; c < g.modelos.size();) {
			glBegin(GL_TRIANGLES);
				glVertex3f(g.modelos[c++], g.modelos[c++], g.modelos[c++]);
				glVertex3f(g.modelos[c++], g.modelos[c++], g.modelos[c++]);
				glVertex3f(g.modelos[c++], g.modelos[c++], g.modelos[c++]);
			glEnd();
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
	// put GLUT’s init here
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("CG-TP");

	// put callback registry here
	glutReshapeFunc(changeSize);
	//glutIdleFunc(renderScene);
	glutDisplayFunc(renderScene);

	// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

	// some OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glPolygonMode(GL_FRONT, GL_LINE);

	// enter GLUT’s main cycle
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
		width = stof(window->Attribute("width"));
		height = stof(window->Attribute("height"));

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

		XMLElement* up = lookat->NextSiblingElement("up");
		//define camera vector
		upCamx = stof(up->Attribute("x"));
		upCamy = stof(up->Attribute("y"));
		upCamz = stof(up->Attribute("z"));

		XMLElement* projection = up->NextSiblingElement("projection");
		//define camera projection
		fov = stof(projection->Attribute("fov"));
		near = stof(projection->Attribute("near"));
		far = stof(projection->Attribute("far"));

		XMLElement* grupo = camera->NextSiblingElement("group");

		while (grupo != nullptr) {
			getGroups(grupo);
			grupo = grupo->NextSiblingElement("group");
		}
		cout << "Tamanho:" << endl;
		cout << my_world.size() << endl;

		/*
		XMLElement* models = group->FirstChildElement("models");
		XMLElement* model = models->FirstChildElement("model");

		while (model != nullptr) {
			modelos.push_back(model->Attribute("file"));
			model = model->NextSiblingElement();
		}*/

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
		//cout << "model1 = " << modelos[0] << endl;

	}
	else {
		cout << "Failed to load XML file" << endl;
	}
}

vector<float> getModels(vector<string> ms) {
	vector<float> vertex;

	for (int c = 0; c < ms.size(); c++) {
		std::ifstream myfile;
		string path = fs::current_path().string();
		string dirpath = path + "\\";
		dirpath += ms[c];
		myfile.open(dirpath);
		string line, seg;
		int vertex_num = 0;

		if (myfile.is_open()) {
			cout << "Reading from: " << ms[c] << endl;

			getline(myfile, seg);

			vertex_num = stoi(seg);

			for (int i = 0; i < vertex_num; i++) {
				getline(myfile, line);
				stringstream tmpss(line);

				for (int c = 0; c < 3; c++) {
					getline(tmpss, seg, ',');
					vertex.push_back(stof(seg));
				}
			}

			cout << "Num.Vertices" << "=>";
			cout << vertex.size() << endl;
			cout << ms[c] << endl;
		}
		else {
			cout << "Failed to open " << ms[c] << endl;
		}
	}

	cout << "Primitivas" << "=>";
	cout << ms.size() << endl;

	return vertex;
}

Group getGroups(XMLElement* xmlelement) {
	float x = 0;
	float y = 0;
	float z = 0;
	float angle = 0;
	vector<Transformation> transformacoes;
	vector<string> modelos;

	XMLElement* transformacao = xmlelement->FirstChildElement("transform");
	XMLElement* translacao = transformacao->FirstChildElement("translate");

	if (translacao != nullptr) {

		if (translacao->Attribute("x") != nullptr) {
			x = stof(translacao->Attribute("x"));
		}
		if (translacao->Attribute("y") != nullptr) {
			y = stof(translacao->Attribute("y"));
		}
		if (translacao->Attribute("z") != nullptr) {
			z = stof(translacao->Attribute("z"));
		}
		Transformation t = new Transformation("translate", x, y, z);
		//cout << TransformationToString(t) << endl;
		transformacoes.push_back(t);
	}

	XMLElement* rotation = transformacao->FirstChildElement("rotate");
	if (rotation != nullptr) {

		if (rotation->Attribute("angle") != nullptr) {
			angle = stof(rotation->Attribute("angle"));
		}
		if (rotation->Attribute("x") != nullptr) {
			x = stof(rotation->Attribute("x"));
		}
		if (rotation->Attribute("y") != nullptr) {
			y = stof(rotation->Attribute("y"));
		}
		if (rotation->Attribute("z") != nullptr) {
			z = stof(rotation->Attribute("z"));
		}
		Transformation t = new Transformation("rotation", angle, x, y, z);
		//cout << TransformationToString(t) << endl;
		transformacoes.push_back(t);

	}
	XMLElement* scalation = transformacao->FirstChildElement("scale");
	if (scalation != nullptr) {
		if (scalation->Attribute("x") != nullptr) {
			x = stof(scalation->Attribute("x"));
		}
		if (scalation->Attribute("y") != nullptr) {
			y = stof(scalation->Attribute("y"));
		}
		if (scalation->Attribute("z") != nullptr) {
			z = stof(scalation->Attribute("z"));
		}
		Transformation t = new Transformation("scale", x, y, z);
		//cout << TransformationToString(t) << endl;
		transformacoes.push_back(t);
	}

	XMLElement* models = xmlelement->FirstChildElement("models");
	XMLElement* model = models->FirstChildElement("model");

	while (model != nullptr) {

		if (model->Attribute("file") != nullptr) {
			allmodels.push_back(model->Attribute("file"));
			modelos.push_back(model->Attribute("file"));
		}
		model = model->NextSiblingElement();
	}

	XMLElement* childElement = xmlelement->FirstChildElement("group");

	Group groupElement = Group(transformacoes, getModels(modelos));

	while (childElement != nullptr) {
		cout << "entrou" << endl;
		Group chld = getGroups(childElement);
		//get the nested children group
		groupElement.addChild(chld);
		childElement = childElement->FirstChildElement("group");
	}

	cout << "modelos:" + getModelos(modelos) << endl;

	if (testing_var == 0) {
		my_world.push_back(groupElement);
		testing_var++;
	}

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

string PrimitivaToString(Primitiva p){
	return p.filename;
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
