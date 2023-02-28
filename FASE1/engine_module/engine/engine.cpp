#include "tinyxml2.h"
#include "engine.h"

#define _USE_MATH_DEFINES
using namespace tinyxml2;

vector<Triangle> triangulos;
int sizeTriangulos = 0;

vector<string> modelos;
vector<Point> vertex;
float width = 0, height = 0;
float posCamx = 0, posCamy = 0, posCamz = 0;
float lookCamx = 0, lookCamy = 0, lookCamz = 0;
float upCamx = 0, upCamy = 0, upCamz = 0;
float fov = 0, near = 0, far = 0;


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
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void renderScene(void)
{
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// set camera
	glLoadIdentity();
	gluLookAt(posCamx, posCamy, posCamz,
		lookCamx, lookCamy, lookCamz,
		upCamx, upCamy, upCamz);
	glColor3f(1.0f, 1.0f, 1.0f);

	
	// End of frame
	glutSwapBuffers();
}


int main(int argc, char** argv)
{
	parse_XML();
	// put GLUT�s init here
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("CG1");

	// put callback registry here
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutDisplayFunc(renderScene);

	// some OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// enter GLUT�s main cycle
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
			string seg;
			int vertex_num = 0;

			if (myfile.is_open()) {
				getline(myfile, seg);

				vertex_num = stoi(seg);

				vector<int> tmp;

				for (int i = 0; i < vertex_num; i++) {
					while (getline(myfile, seg, ',')) 
						tmp.push_back(stoi(seg));

					vertex.push_back(Point(tmp[0], tmp[1], tmp[2], tmp[3]));
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
