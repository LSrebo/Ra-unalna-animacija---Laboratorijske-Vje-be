#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#include <string>
#include <fstream>
#include <vector>
#include <iostream>

#define _CRT_SECURE_NO_WARNINGS

#define PI 3.14159265358979323846

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

using namespace std;

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

class Source{
public:
	float x, y, z;
	int quantity;
	float size;

	Source(float _x, float _y, float _z){
		x = _x;
		y = _y;
		z = _z;
		quantity = 1;
		size = 1.0;
	}

	void setQuantity(int _quantity) {
		quantity = _quantity;
	}

	void setSize(float _size) {
		size = _size;
	}
};

class Particle {
public:
	float x, y, z;
	float r, g, b;
	float directionX, directionY, directionZ;
	float velocity;
	float acceleration;
	int lifetime;
	float Xaxis, Yaxis, Zaxis;
	float angle;
	float size;

	Particle(float _x, float _y, float _z) {
		setXYZ(_x, _y, _z);
	}

	Particle(float _x, float _y, float _z, float _r, float _g, float _b) {
		setXYZ(_x, _y, _z);
		setRGB(_r, _g, _b);
	}
	
	void setXYZ(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	void setRGB(float _r, float _g, float _b) {
		r = _r;
		g = _g;
		b = _b;
	}

	void setDirection(float _x, float _y, float _z) {
		directionX = _x;
		directionY = _y;
		directionZ = _z;
	}

	void setVelocity(float _velocity) {
		velocity = _velocity;
	}

	void setAcceleration(float _acceleration) {
		acceleration = _acceleration;
	}

	void setLifetime(int _lifetime) {
		lifetime = _lifetime;
	}

	void setAxis(float _x, float _y, float _z) {
		Xaxis = _x;
		Yaxis = _y;
		Zaxis = _z;
	}

	void setAngle(float _angle) {
		angle = _angle;
	}

	void setSize(float _size) {
		size = _size;
	}
};

GLuint window;
GLuint width = 800, height = 600;

GLuint LoadTexture(const char* filename);

void myDisplay();
void myReshape(int w, int h);
void myMouse(int button, int state, int x, int y);
void myKeyboard(unsigned char key, int mouseX, int mouseY);
void myIdle();

void drawParticles();

GLuint texture;

Vertex ociste(0.0, 0.0, 75.0);
Source source(0, 0, 0);

float mainColorR = 0.0;
float mainColorG = 1.0;
float mainColorB = 0.0;

vector<Particle> particles;

int currentTime = 0;
int previousTime = 0;

int main(int argc, char** argv)
{
	source.setQuantity(5);
	source.setSize(0.5);
	srand(time(NULL));

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutInit(&argc, argv);

	window = glutCreateWindow("2.labos");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);
	glutIdleFunc(myIdle);

	texture = LoadTexture("cestica.bmp");

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glutMainLoop();
	return 0;
}

GLuint LoadTexture(const char* filename)
{
	GLuint texture;
	int width, height;
	BYTE* data;
	FILE* file;

	if (fopen_s(&file, filename, "rb") != 0) {
		cout << "Couldn't find the file!" << endl;
		return 0;
	}

	width = 256;
	height = 256;
	data = (BYTE*)malloc(width * height * 2);

	fread(data, width * height * 2, 1, file);
	fclose(file);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	return texture;
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

	glLoadIdentity();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(1.0);
	glColor3f(0.0f, 0.0f, 0.0f);
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(ociste.x, ociste.y, -ociste.z);
	drawParticles();
	glutSwapBuffers();
}

void myIdle() {
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	int timeInterval = currentTime - previousTime;
	if (timeInterval > 100) {
		if (source.quantity > 0) {
			for (int i = 0; i < source.quantity; i++) {
				double x, y, z;
				x = (rand() / (double)RAND_MAX) * 2.0 - 1.0;
				y = (rand() / (double)RAND_MAX) * 2.0 - 1.0;
				z = (rand() / (double)RAND_MAX) * 2.0 - 1.0;

				Particle particle(source.x, source.y, source.z, mainColorR, mainColorG, mainColorB);
				particle.setVelocity(0.5);
				particle.setAcceleration(0.01);
				particle.setDirection(x, y, z);
				particle.setLifetime(75);
				particle.setSize(source.size);
				particles.push_back(particle);
			}
		}
		for (int i = 0; i < particles.size(); i++) {
			Vertex s(0.0, 0.0, 0.0);
			Vertex os(0.0, 0.0, 0.0);
			Vertex e(0.0, 0.0, 0.0);

			s.x = particles[i].x;
			s.y = particles[i].y;
			s.z = particles[i].z;

			e.x = ociste.x;
			e.y = ociste.y;
			e.z = ociste.z;

			os.x = s.y * e.z - e.y * s.z;
			os.y = e.x * s.z - s.x * e.z;
			os.z = s.x * e.y - s.y * e.x;

			double absS = pow(pow((double)s.x, 2.0) + pow((double)s.y, 2.0) + pow((double)s.z, 2.0), 0.5);
			double absE = pow(pow((double)e.x, 2.0) + pow((double)e.y, 2.0) + pow((double)e.z, 2.0), 0.5);
			double se = s * e;
			double angle = acos(se / (absS * absE));
			particles.at(i).setAngle(angle * (180 / PI));
			particles.at(i).setAxis(os.x, os.y, os.z);

			if (particles.at(i).b < 1.0) particles.at(i).b += 0.02;
			if (particles.at(i).b < 1.0) particles.at(i).r += 0.02;
			if (particles.at(i).size >= 0.5) particles.at(i).size += 0.01;

			particles.at(i).x += particles.at(i).velocity * particles.at(i).directionX;
			particles.at(i).y += particles.at(i).velocity * particles.at(i).directionY;
			particles.at(i).z += particles.at(i).velocity * particles.at(i).directionZ;
			
			particles.at(i).velocity += particles.at(i).acceleration;

			particles.at(i).lifetime--;

			if (particles.at(i).lifetime <= 0) {
				particles.erase(particles.begin() + i);
			}
		}

		myDisplay();
		previousTime = currentTime;
	}
}

void myMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		source.x = x;
		source.y = y;
	}

}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {
	if (theKey == '+') source.quantity++;
	if (theKey == '-') source.quantity--;

	if (theKey == 'a') ociste.x += 0.2;
	if (theKey == 'd') ociste.x -= 0.2;
	if (theKey == 's') ociste.y += 0.2;
	if (theKey == 'w') ociste.y -= 0.2;
}

void drawParticles() {
	for (int i = 0; i < particles.size(); i++) {
		glColor3f(particles[i].r, particles[i].g, particles[i].b);
		glTranslatef(particles[i].x, particles[i].y, particles[i].z);
		glRotatef(particles[i].angle, particles[i].Xaxis, particles[i].Yaxis, particles[i].Zaxis);
		glBegin(GL_QUADS);

		glTexCoord2d(0.0, 0.0); glVertex3f(-particles[i].size, -particles[i].size, 0.0);
		glTexCoord2d(1.0, 0.0); glVertex3f(particles[i].size, -particles[i].size, 0.0);
		glTexCoord2d(1.0, 1.0); glVertex3f(particles[i].size, particles[i].size, 0.0);
		glTexCoord2d(0.0, 1.0); glVertex3f(-particles[i].size, particles[i].size, 0.0);

		glEnd();
		glRotatef(-particles[i].angle, particles[i].Xaxis, particles[i].Yaxis, particles[i].Zaxis);
		glTranslatef(-particles[i].x, -particles[i].y, -particles[i].z);
	}
}