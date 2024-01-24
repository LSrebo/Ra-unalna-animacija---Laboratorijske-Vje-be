#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>

#define PI 3.14159265358979323846

using namespace std;

int numOfFaces = 0;
int numOfVertices = 0;
int numOfBVert = 0;
int numOfBSeg;

int multiplier = 10;


class Vertex {
public:
	float x, y, z;
	Vertex() {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
	Vertex(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vertex& operator+=(const Vertex& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vertex& operator /=(float scalar) {
		if (scalar != 0.0f) {
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}
		else {
			return *this;
		}
	}

	Vertex operator+(const Vertex& other) const {
		return Vertex(x + other.x, y + other.y, z + other.z);
	}

	Vertex operator-(const Vertex& other) const {
		return Vertex(x - other.x, y - other.y, z - other.z);
	}

	Vertex operator*(float scalar) const {
		return Vertex(x * scalar, y * scalar, z * scalar);
	}

	float operator*(const Vertex& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	Vertex operator/(float scalar) const {
		if (scalar != 0.0f) {
			return Vertex(x / scalar, y / scalar, z / scalar);
		}
		else {
			return *this;
		}
	}

	friend std::ostream& operator<<(std::ostream& os, const Vertex& vertex) {
		os << "Vertex(" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")";
		return os;
	}
};

class Face {
public:
	Vertex v1, v2, v3;
	Face(Vertex _v1, Vertex _v2, Vertex _v3) {
		v1 = _v1;
		v2 = _v2;
		v3 = _v3;
	}
};

vector<Vertex> vertices;
vector<Face> faces;
vector<Vertex> vertices_b;
vector<Vertex> bSpline;
vector<Vertex> tanB;
vector<Vertex> allTanB;

Vertex center;
Vertex s(0.0, 0.0, 1.0);
Vertex e(0.0, 0.0, 0.0);
Vertex os(0.0, 0.0, 0.0);

GLuint window;
GLuint width = 800, height = 600;

void myDisplay();
void myReshape(int w, int h);
void myMouse(int button, int state, int x, int y);
void myKeyboard(unsigned char key, int mouseX, int mouseY);
void animate(int value);

int main(int argc, char** argv) {

	vector<string> file_lines;

	ifstream ifs("objekti/f16.obj");

	string line;
	while (getline(ifs, line)) {
		file_lines.push_back(line);
	}

	for (int i = 0; i < file_lines.size(); i++) {
		if (!file_lines.at(i).empty()) {
			if (file_lines.at(i).at(0) == 'v') {
				numOfVertices++;
			}
			else if (file_lines.at(i).at(0) == 'f') {
				numOfFaces++;
			}
		}
	}

	float minx, maxx, miny, maxy, minz, maxz;
	bool first = true;

	for (int i = 0; i < file_lines.size(); i++) {
		if (!file_lines.at(i).empty()) {
			if (file_lines.at(i).at(0) == 'v') {
				vector<float> coords;
				istringstream iss(file_lines.at(i));
				string token;
				int iter = 0;

				iss >> token;

				while (iss >> token) {
					try {
						float value = stof(token);
						if (first) {
							if (iter == 0) {
								minx = value;
								maxx = value;
							}
							else if (iter == 1) {
								miny = value;
								maxy = value;
							}
							else {
								minz = value;
								maxz = value;
							}
						}
						else {
							if (iter == 0) {
								if (value < minx) minx = value;
								if (value > maxx) maxx = value;
							}
							else if (iter == 1) {
								if (value < miny) miny = value;
								if (value > maxy) maxy = value;
							}
							else {
								if (value < minz) minz = value;
								if (value > maxz) maxz = value;
							}
						}
						iter++;
						coords.push_back(value);
					}
					catch (const invalid_argument& e) {

					}
				}
				first = false;

				Vertex v(coords.at(0), coords.at(1), coords.at(2));
				center += v;
				vertices.push_back(v);
			}
		}
	}

	float rangex = maxx - minx;
	float rangey = maxy - miny;
	float rangez = maxz - minz;

	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].x = multiplier * ((vertices[i].x - minx) / rangex);
		vertices[i].y = multiplier * ((vertices[i].y - miny) / rangey);
		vertices[i].z = multiplier * ((vertices[i].z - minz) / rangez);
	}

	center /= vertices.size();

	for (int i = 0; i < file_lines.size(); i++) {
		if (!file_lines.at(i).empty()) {
			if (file_lines.at(i).at(0) == 'f') {
				vector<int> faces_index;
				istringstream iss(file_lines.at(i));
				string token;

				iss >> token;

				while (iss >> token) {
					try {
						int value = stoi(token);
						faces_index.push_back(value);
					}
					catch (const invalid_argument& e) {

					}
				}

				Face f(vertices.at(faces_index.at(0) - 1), vertices.at(faces_index.at(1) - 1), vertices.at(faces_index.at(2) - 1));
				faces.push_back(f);
			}
		}
	}

	file_lines.clear();
	ifstream ifs_b("b_spline.txt");

	while (getline(ifs_b, line)) {
		file_lines.push_back(line);
	}

	numOfBVert = file_lines.size();
	numOfBSeg = numOfBVert - 3;

	float minBx, maxBx, minBy, maxBy, minBz, maxBz;

	for (int i = 0; i < file_lines.size(); i++) {
		if (!file_lines.at(i).empty()) {
			vector<float> coords;
			istringstream iss(file_lines.at(i));
			string token;
			int iter = 0;

			while (iss >> token) {
				try {
					float value = stof(token);
					if (i == 0) {
						if (iter == 0) {
							minBx = value;
							maxBx = value;
						}
						else if (iter == 1) {
							minBy = value;
							maxBy = value;
						}
						else {
							minBz = value;
							maxBz = value;
						}
					}
					else {
						if (iter == 0) {
							if (value < minBx) minBx = value;
							if (value > maxBx) maxBx = value;
						}
						else if (iter == 1) {
							if (value < minBy) minBy = value;
							if (value > maxBy) maxBy = value;
						}
						else {
							if (value < minBz) minBz = value;
							if (value > maxBz) maxBz = value;
						}
					}
					iter++;
					coords.push_back(value);
				}
				catch (const invalid_argument& e) {

				}
			}

			Vertex v(coords.at(0), coords.at(1), coords.at(2));
			vertices_b.push_back(v);
		}
	}

	float rangeBx = maxBx - minBx;
	float rangeBy = maxBy - minBy;
	float rangeBz = maxBz - minBz;

	/*for (int i = 0; i < vertices_b.size(); i++) {
		vertices_b[i].x = multiplier * ((vertices_b[i].x - minBx) / rangeBx);
		vertices_b[i].y = multiplier * ((vertices_b[i].y - minBy) / rangeBy);
		vertices_b[i].z = multiplier * ((vertices_b[i].z - minBz) / rangeBz);
	}*/

	bSpline.resize(100 * numOfBSeg);

	for (int i = 0; i < numOfBSeg; i++) {
		Vertex v0 = vertices_b[i];
		Vertex v1 = vertices_b[i + 1];
		Vertex v2 = vertices_b[i + 2];
		Vertex v3 = vertices_b[i + 3];

		for (int j = 0; j < 100; j++) {
			float t = (float)j / (float)100;

			float r1 = (-pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1) / 6;
			float r2 = (3 * pow(t, 3) - 6 * pow(t, 2) + 4) / 6;
			float r3 = (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) / 6;
			float r4 = pow(t, 3) / 6;

			Vertex bSplineNow = Vertex(r1 * v0.x + r2 * v1.x + r3 * v2.x + r4 * v3.x, r1 * v0.y + r2 * v1.y + r3 * v2.y + r4 * v3.y, r1 * v0.z + r2 * v1.z + r3 * v2.z + r4 * v3.z);

			bSpline[100 * i + j] = bSplineNow;

			float t1 = (-pow(t, 2) + 2 * t - 1) / 2;
			float t2 = (3 * pow(t, 2) - 4 * t) / 2;
			float t3 = (-3 * pow(t, 2) + 2 * t + 1) / 2;
			float t4 = (pow(t, 2)) / 2;
			Vertex tan = Vertex(t1 * v0.x + t2 * v1.x + t3 * v2.x + t4 * v3.x, t1 * v0.y + t2 * v1.y + t3 * v2.y + t4 * v3.y, t1 * v0.z + t2 * v1.z + t3 * v2.z + t4 * v3.z);
			float sum = pow(pow(tan.x, 2) + pow(tan.y, 2) + pow(tan.z, 2), 0.5);
			tan /= sum;
			allTanB.push_back(bSplineNow);
			allTanB.push_back(bSplineNow + (tan / 4));
			if (j % 10 == 0) {
				tanB.push_back(bSplineNow);
				tanB.push_back(bSplineNow + (tan / 4));
			}
		}
	}

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutInit(&argc, argv);

	window = glutCreateWindow("1.labos");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);
	glutTimerFunc(20, animate, 0);

	glutMainLoop();
	return 0;
}

void myReshape(int w, int h)
{
	width = w;
	height = h;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 10.0, 1.0, 0.0, 0.0);

	glLoadIdentity();
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(1.0);
	glColor3f(0.0f, 0.0f, 0.0f);
}

int step = 0;
float pi = 3.14159265;

void myDisplay() {
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTranslatef(-5.0, -5.0, -75.0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < numOfBSeg * 100; i++) {
		glVertex3f(bSpline[i].x, bSpline[i].y, bSpline[i].z);
	}
	glEnd();
	glBegin(GL_LINES);
	for (int i = 0; i < numOfBSeg * 20; i += 2) {
		glVertex3f(tanB[i].x, tanB[i].y, tanB[i].z);
		glVertex3f(tanB[i + 1].x, tanB[i + 1].y, tanB[i + 1].z);
	}
	glEnd();

	e = allTanB[2 * step + 1] - allTanB[2 * step];

	os.x = s.y * e.z - e.y * s.z;
	os.y = -(s.x * e.z - e.x * s.z);
	os.z = s.x * e.y - e.x * s.y;

	double absS = pow(pow((double)s.x, 2) + pow((double)s.y, 2) + pow((double)s.z, 2), 0.5);
	double absE = pow(pow((double)e.x, 2) + pow((double)e.y, 2) + pow((double)e.z, 2), 0.5);
	double se = s * e;

	double angle = acos(se / (absS * absE));
	angle = angle * (180 / PI);
	glRotatef(angle, os.x, os.y, os.z);
	glTranslatef(allTanB[2 * step].x, allTanB[2 * step].y, allTanB[2 * step].z);

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < faces.size(); i++) {
		Vertex v1 = faces[i].v1;
		Vertex v2 = faces[i].v2;
		Vertex v3 = faces[i].v3;

		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v3.x, v3.y, v3.z);
	}
	glEnd();

	glFlush();

}

void myMouse(int button, int state, int x, int y)
{

}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{

}

void animate(int value) {
	step++;
	if (step == (100 * numOfBSeg) - 1) {
		step = 0;
	}
	glutPostRedisplay();
	glutTimerFunc(20, animate, 0);
}