#include "tinyxml2.h"
#include "engine.h"

#define M_FIX 0
#define M_FPS 1
#define STEP ((M_PI*2) / 20)
using namespace tinyxml2;

vector<Triangle> triangulos;
int sizeTriangulos = 0;
int mode = M_FIX;

vector<string> modelos;
vector<Point> vertex;
float width = 0, height = 0;
float posCamx = 0, posCamy = 0, posCamz = 0;
float lookCamx = 0, lookCamy = 0, lookCamz = 0;
float upCamx = 0, upCamy = 0, upCamz = 0;
float fov = 0, near = 0, far = 0;
float G_alpha = 0.0f;
float G_beta = 0.0f;
float G_radious = 5.0f;


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
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
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

	// Transformations


	// Models
	build_models();
	
	// End of frame
	glutSwapBuffers();
}

void build_models() {
	Point p1, p2, p3;
	for (int c = 0; c < vertex.size(); c += 3) {
		p1 = vertex[c];
		p2 = vertex[c + 1];
		p3 = vertex[c + 2];
		glBegin(GL_TRIANGLES);
			glColor3f(1.0f, 0.5f, 0.0f);
			glVertex3f(p1.x, p1.y, p1.z);
			glVertex3f(p2.x, p2.y, p2.z);
			glVertex3f(p3.x, p3.y, p3.z);
		glEnd();
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
	if (key == GLUT_KEY_F1) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (key == GLUT_KEY_F2) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (key == GLUT_KEY_F3) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	parse_XML();
	// put GLUT’s init here
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("CG1");

	// put callback registry here
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutDisplayFunc(renderScene);

	// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

	// some OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// enter GLUT’s main cycle
	glutMainLoop();

	return 1;
}

void parse_XML() {
	XMLDocument document;
	std::string path= fs::current_path().string();
	string dir = path.substr(0, path.length() - 5);
	dir += "engine\\config.xml";

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

		XMLElement* group = camera->NextSiblingElement("group");
		XMLElement* models = group->FirstChildElement("models");
		XMLElement* model = models->FirstChildElement("model");

		while (model != nullptr) {
			modelos.push_back(model->Attribute("file"));
			model = model->NextSiblingElement();
		}

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
		cout << "model1 = " << modelos[0] << endl;

	}
	else {
		cout << "Failed to load XML file" << endl;
	}

	if(modelos.size() > 0)
		for (string fname : modelos) {
			std::ifstream myfile;
			string path = fs::current_path().string();
			path += "\\";
			path += fname;
			myfile.open(path);
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

					vertex.push_back(Point(tmp[0], tmp[1], tmp[2], 0)); // TODO NEED TO CHANGE GENERATOR SO IT ALSO WRITES AST VERTIX POINT
					tmp.clear();
				}

				for (Point p : vertex) {
					cout << p.x << "," << p.y << "," << p.z << "," << p.dim << endl;
				}
			}
			else {
				cout << "Failed to open " << fname << endl;
			}
		}
}
