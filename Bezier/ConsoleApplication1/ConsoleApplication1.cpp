


#pragma region DECLARATIONS

#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <stack>
#include <list>
#include <algorithm>
#include "Matrix4.h"

using namespace std;

#define PI 3.14159265


// Struct
struct point
{
	float x, y;

	point() {};
	point(float _x, float _y) : x(_x), y(_y) {}

	void operator= (const point& a) {
		x = a.x;
		y = a.y;
	}
	point operator- () {
		return point(-x, -y);
	}
	float magnitude()
	{
		return sqrt(x*x + y*y);
	}
	point normalised()
	{
		float m = magnitude();
		return point(x/m, y/m);
	}
};
inline point operator+ (const point& a, const point& b) {
	point p;
	p.x = a.x + b.x;
	p.y = a.y + b.y;
	return p;
}

inline point operator* (const point& a, const float k) {
	point p;
	p.x = a.x * k;
	p.y = a.y * k;
	return p;
}
inline point operator- (const point& a, const point& b) {
	point p;
	p.x = a.x - b.x;
	p.y = a.y - b.y;
	return p;
}
inline bool operator== (const point& a, const point& b) {
	return (abs(a.x - b.x) < 0.001f && abs(a.y - b.y) < 0.001f);
}
inline bool operator!= (const point& a, const point& b) {
	return (abs(a.x - b.x) > 0.001f || abs(a.y - b.y) > 0.001f);
}



struct vec4
{
	float x, y, z, w;

	vec4(){};
	vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
};
inline vec4 operator *(const Matrix4& m, const vec4& d) {
	vec4 result;

	result.x = (m.m00 * d.x) + (m.m01 * d.y) + (m.m02 * d.z) + (m.m03 * d.w);
	result.y = (m.m10 * d.x) + (m.m11 * d.y) + (m.m12 * d.z) + (m.m13 * d.w);
	result.z = (m.m20 * d.x) + (m.m21 * d.y) + (m.m22 * d.z) + (m.m23 * d.w);
	result.w = (m.m30 * d.x) + (m.m31 * d.y) + (m.m32 * d.z) + (m.m33 * d.w);

	return result;
}

inline point operator+(const point& a, const vec4& d) {
	Matrix4 m = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	vec4 result;

	m.m03 = a.x;
	m.m13 = a.y;
	m.m23 = 0.0f;
	m.m33 = 1.0f;

	result = m * d;

	point p;
	p.x = result.x;
	p.y = result.y;

	return p;
}

inline point operator-(const point& a, const vec4& d) {
	Matrix4 m = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	vec4 sd;

	sd.x = a.x;
	sd.y = a.y;
	sd.z = 0.0f;
	sd.w = 1.0f;

	m.m00 = cosf(d.z * PI / 180.0);
	m.m01 = -sinf(d.z * PI / 180.0);
	m.m10 = sinf(d.z * PI / 180.0);
	m.m11 = cosf(d.z * PI / 180.0);

	vec4 result;
	result = m * sd;

	point p;
	p.x = result.x;
	p.y = result.y;

	return p;
}

inline point operator*(const point& a, const vec4& d) {
	Matrix4 m = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	vec4 sd;

	sd.x = a.x;
	sd.y = a.y;
	sd.z = 0.0f;
	sd.w = 1.0f;

	m.m00 = d.x;
	m.m11 = d.y;
	m.m22 = d.z;
	m.m33 = 1.0f;

	vec4 result;
	result = m * sd;

	point p;
	p.x = result.x;
	p.y = result.y;

	return p;
}


vector<vector<vector<point>>> curves;
bool Intersection(point a, point b, point c, point d, point* i);
point inter;

class Raccordement
{
public:
	int bezierA, bezierB;
	virtual void Init() {}
	virtual void Update() {}
};

class C0 : public Raccordement
{
public:
	point lastPosition;

	void Init()
	{
		int lastA = curves[0][bezierA].size() - 1;
		lastPosition = (curves[0][bezierA][lastA] + curves[0][bezierB][0]) * 0.5f;
		curves[0][bezierA][lastA] = lastPosition;
		curves[0][bezierB][0] = lastPosition;
	}

	void Update()
	{
		int lastA = curves[0][bezierA].size() - 1;
		if (curves[0][bezierA][lastA] != lastPosition)
			lastPosition = curves[0][bezierA][lastA];
		else if (curves[0][bezierB][0] != lastPosition)
			lastPosition = curves[0][bezierB][0];
		curves[0][bezierA][lastA] = lastPosition;
		curves[0][bezierB][0] = lastPosition;
	}
};

class C1 : public Raccordement
{
public:
	point lastPosition;
	point lastPositionControleADelta;
	point lastPositionControleBDelta;

	void Init()
	{
		int lastA = curves[0][bezierA].size() - 1;
		lastPosition = (curves[0][bezierA][lastA] + curves[0][bezierB][0]) * 0.5f;
		curves[0][bezierA][lastA] = lastPosition;
		curves[0][bezierB][0] = lastPosition;
		lastPositionControleADelta = curves[0][bezierA][lastA - 1] - curves[0][bezierA][lastA];
		lastPositionControleBDelta = curves[0][bezierB][1] - curves[0][bezierB][0];
		float r1 = lastPositionControleADelta.magnitude();
		float r2 = lastPositionControleBDelta.magnitude();
		point direction = ((curves[0][bezierA][lastA - 1] - curves[0][bezierA][lastA]) + -(curves[0][bezierB][1] - curves[0][bezierB][0])) * 0.5f;
		direction = direction.normalised();
		lastPositionControleADelta = direction * r1;
		lastPositionControleBDelta = -direction * r2;
		curves[0][bezierA][lastA - 1] = curves[0][bezierA][lastA] + lastPositionControleADelta;
		curves[0][bezierB][1] = curves[0][bezierB][0] + lastPositionControleBDelta;
	}

	void Update()
	{
		int lastA = curves[0][bezierA].size() - 1;
		if (curves[0][bezierA][lastA] != lastPosition || curves[0][bezierB][0] != lastPosition) {
			if (curves[0][bezierA][lastA] != lastPosition)
				lastPosition = curves[0][bezierA][lastA];
			else if (curves[0][bezierB][0] != lastPosition)
				lastPosition = curves[0][bezierB][0];
			curves[0][bezierA][lastA] = lastPosition;
			curves[0][bezierB][0] = lastPosition;
			curves[0][bezierA][lastA - 1] = curves[0][bezierA][lastA] + lastPositionControleADelta;
			curves[0][bezierB][1] = curves[0][bezierB][0] + lastPositionControleBDelta;
		}
		else if ((curves[0][bezierA][lastA - 1] - curves[0][bezierA][lastA]) != lastPositionControleADelta) {
			float r2 = lastPositionControleBDelta.magnitude();
			lastPositionControleADelta = curves[0][bezierA][lastA - 1] - curves[0][bezierA][lastA];
			lastPositionControleBDelta = -lastPositionControleADelta.normalised() * r2;
			curves[0][bezierB][1] = curves[0][bezierB][0] + lastPositionControleBDelta;
		}
		else if ((curves[0][bezierB][1] - curves[0][bezierB][0]) != lastPositionControleBDelta) {
			float r1 = lastPositionControleADelta.magnitude();
			lastPositionControleBDelta = curves[0][bezierB][1] - curves[0][bezierB][0];
			lastPositionControleADelta = -lastPositionControleBDelta.normalised() * r1;
			curves[0][bezierA][lastA - 1] = curves[0][bezierA][lastA] + lastPositionControleADelta;
		}
	}
};

class C2 : public Raccordement
{
public:
	point lastPosition;
	point lastPositionControleA1Delta;
	point lastPositionControleB1Delta;
	point lastPositionControleA2Delta;
	point lastPositionControleB2Delta;

	void Init()
	{
		int lastA = curves[0][bezierA].size() - 1;
		lastPosition = (curves[0][bezierA][lastA] + curves[0][bezierB][0]) * 0.5f;
		curves[0][bezierA][lastA] = lastPosition;
		curves[0][bezierB][0] = lastPosition;
		
		lastPositionControleA1Delta = curves[0][bezierA][lastA - 1] - curves[0][bezierA][lastA];
		lastPositionControleB1Delta = curves[0][bezierB][1] - curves[0][bezierB][0];
		float r1 = lastPositionControleA1Delta.magnitude();
		float r2 = lastPositionControleB1Delta.magnitude();
		point direction = ((curves[0][bezierA][lastA - 1] - curves[0][bezierA][lastA]) + -(curves[0][bezierB][1] - curves[0][bezierB][0])) * 0.5f;
		direction = direction.normalised();
		lastPositionControleA1Delta = direction * r1; 
		lastPositionControleB1Delta = -direction * r2;
		curves[0][bezierA][lastA - 1] = curves[0][bezierA][lastA] + lastPositionControleA1Delta;
		curves[0][bezierB][1] = curves[0][bezierB][0] + lastPositionControleB1Delta;

		point i;
		if (Intersection(curves[0][bezierA][lastA - 2], curves[0][bezierA][lastA - 1], curves[0][bezierB][1], curves[0][bezierB][2], &i)) {
			lastPositionControleA2Delta = (curves[0][bezierA][lastA - 1] - i) * (r1 / r2);
			lastPositionControleB2Delta = (curves[0][bezierB][1] - i) * (r2 / r1);
			curves[0][bezierA][lastA - 2] = curves[0][bezierA][lastA - 1] + lastPositionControleA2Delta;
			curves[0][bezierB][2] = curves[0][bezierB][1] + lastPositionControleB2Delta;
			inter = i;
		}
	}

	void Update()
	{
		int lastA = curves[0][bezierA].size() - 1;
		if (curves[0][bezierA][lastA] != lastPosition || curves[0][bezierB][0] != lastPosition) {
			if (curves[0][bezierA][lastA] != lastPosition)
				lastPosition = curves[0][bezierA][lastA];
			else if (curves[0][bezierB][0] != lastPosition)
				lastPosition = curves[0][bezierB][0];
			curves[0][bezierA][lastA] = lastPosition;
			curves[0][bezierB][0] = lastPosition;
			curves[0][bezierA][lastA - 1] = curves[0][bezierA][lastA] + lastPositionControleA1Delta;
			curves[0][bezierB][1] = curves[0][bezierB][0] + lastPositionControleB1Delta;
			curves[0][bezierA][lastA - 2] = curves[0][bezierA][lastA-1] + lastPositionControleA2Delta;
			curves[0][bezierB][2] = curves[0][bezierB][1] + lastPositionControleB2Delta;
		}
		else if ((curves[0][bezierA][lastA - 1] - curves[0][bezierA][lastA]) != lastPositionControleA1Delta) {
			float r2 = lastPositionControleB1Delta.magnitude();
			lastPositionControleA1Delta = curves[0][bezierA][lastA - 1] - curves[0][bezierA][lastA];
			lastPositionControleB1Delta = -lastPositionControleA1Delta.normalised() * r2;
			curves[0][bezierB][1] = curves[0][bezierB][0] + lastPositionControleB1Delta;
			float r1 = lastPositionControleA1Delta.magnitude();
			point i;
			if (Intersection(curves[0][bezierA][lastA - 2], curves[0][bezierA][lastA - 1], curves[0][bezierB][1], curves[0][bezierB][2], &i)) {
				lastPositionControleA2Delta = (curves[0][bezierA][lastA - 1] - i) * (r1 / r2);
				lastPositionControleB2Delta = (curves[0][bezierB][1] - i) * (r2 / r1);
				curves[0][bezierA][lastA - 2] = curves[0][bezierA][lastA - 1] + lastPositionControleA2Delta;
				curves[0][bezierB][2] = curves[0][bezierB][1] + lastPositionControleB2Delta;
				inter = i;
			}
		}
		else if ((curves[0][bezierB][1] - curves[0][bezierB][0]) != lastPositionControleB1Delta) {
			float r1 = lastPositionControleA1Delta.magnitude();
			lastPositionControleB1Delta = curves[0][bezierB][1] - curves[0][bezierB][0];
			lastPositionControleA1Delta = -lastPositionControleB1Delta.normalised() * r1;
			curves[0][bezierA][lastA - 1] = curves[0][bezierA][lastA] + lastPositionControleA1Delta;
			float r2 = lastPositionControleB1Delta.magnitude();
			point i;
			if (Intersection(curves[0][bezierA][lastA - 2], curves[0][bezierA][lastA - 1], curves[0][bezierB][1], curves[0][bezierB][2], &i)) {
				lastPositionControleA2Delta = (curves[0][bezierA][lastA - 1] - i) * (r1 / r2);
				lastPositionControleB2Delta = (curves[0][bezierB][1] - i) * (r2 / r1);
				curves[0][bezierA][lastA - 2] = curves[0][bezierA][lastA - 1] + lastPositionControleA2Delta;
				curves[0][bezierB][2] = curves[0][bezierB][1] + lastPositionControleB2Delta;
				inter = i;
			}
		}
		else if ((curves[0][bezierA][lastA - 2] - curves[0][bezierA][lastA - 1]) != lastPositionControleA2Delta) {
			float r1 = lastPositionControleA1Delta.magnitude();
			float r2 = lastPositionControleB1Delta.magnitude();
			lastPositionControleA2Delta = curves[0][bezierA][lastA - 2] - curves[0][bezierA][lastA - 1];
			point i = curves[0][bezierA][lastA - 1] + -lastPositionControleA2Delta * (r2 / r1);
			lastPositionControleB2Delta = (curves[0][bezierB][1] - i) * (r2 / r1);
			curves[0][bezierB][2] = curves[0][bezierB][1] + lastPositionControleB2Delta;
			inter = i;
		}
		else if ((curves[0][bezierB][2] - curves[0][bezierB][1]) != lastPositionControleB2Delta) {
			float r1 = lastPositionControleA1Delta.magnitude();
			float r2 = lastPositionControleB1Delta.magnitude();
			lastPositionControleB2Delta = curves[0][bezierB][2] - curves[0][bezierB][1];
			point i = curves[0][bezierB][1] + -lastPositionControleB2Delta * (r1 / r2);
			lastPositionControleA2Delta = (curves[0][bezierA][lastA - 1] - i) * (r1 / r2);
			curves[0][bezierA][lastA - 2] = curves[0][bezierA][lastA - 1] + lastPositionControleA2Delta;
			inter = i;
		}
	}
};


C0 C0s[20];
int nbC0 = 0;
C1 C1s[20];
int nbC1 = 0;
C2 C2s[20];
int nbC2 = 0;
vector<Raccordement*> raccordements;


// Polygons variables
int mode = 0;

vector<float>* BaseColor = new vector<float>;
vector<float>* DeCastelColor = new vector<float>;
vector<float>* SplineColor = new vector<float>;

// Window variables
int windowWidth = 800;
int windowHeight = 800;

// States variables
int pickedCurveType = -1;
int pickedCurve = -1;
int pickedPoint = -1;
bool pasAdaptatif = false;
float pas = 10;
Raccordement* newRaccordement;
bool raccordementStep = false;

// Prototypes
void InitVariables();
void Display(void);
void DrawPolygonWire(vector<point> polygon, float r, float g, float b);
void KeyBoard(unsigned char touche, int x, int y);
void Mouse(int bouton, int etat, int x, int y); 
void MouseMove(int x, int y);
void AddMenu();

void SelectVoid(int i);
void SelectMain(int selection);
void SelectCurves(int selection);
void SelectTranformation(int selection);
void SelectBaseColor(int selection);
void SelectDeCastelColor(int selection);
void SelectSplineColor(int selection);
void SelectRaccordement(int selection);
void ClearMode();

vector<float>* SelectColor(int selection);
void Translation(int curveType, int curve);
void Translation(int curveType, int curve, float x, float y);
void Rotation(int curveType, int curve);
void Scaling(int curveType, int curve);
point BaryCentre(vector<point> points);

vector<point> DeCasteljau(vector<point>);
point GetBezierPoint(float f, vector<point> polygon);
float Distance(point a, point b);
vector<point> Spline(vector<point> polygon, vector<point>* construc);

#pragma endregion DECLARATIONS



#pragma region MAIN


int main(int argc, char **argv)
{  
	// Init glut
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Bezier/Spline"); 
	gluOrtho2D(0, windowWidth, windowHeight, 0);

	// Init GL
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glPointSize(5.0);

	// Init glut functions
	glutDisplayFunc(Display);
	glutKeyboardFunc(KeyBoard);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMove);

	// Init variables
	InitVariables();

	// Init menu
	AddMenu();

	glutMainLoop();

	return 0;
}


#pragma endregion MAIN



#pragma region GLUT


void Display()
{
	for (int i = 0; i < raccordements.size(); ++i)
	{
		raccordements[i]->Update();
	}

	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < curves[0].size(); ++i)
	{
		DrawPolygonWire(curves[0][i], BaseColor->at(0), BaseColor->at(1), BaseColor->at(2));
		if (curves[0][i].size() > 2) {
			vector<point> bezier = DeCasteljau(curves[0][i]);
			DrawPolygonWire(bezier, DeCastelColor->at(0), DeCastelColor->at(1), DeCastelColor->at(2));
		}
	}

	for (int i = 0; i < curves[1].size(); ++i)
	{
		DrawPolygonWire(curves[1][i], BaseColor->at(0), BaseColor->at(1), BaseColor->at(2));
		if (curves[1][i].size() > 2) {
			vector<point> construc;
			vector<point> spline = Spline(curves[1][i], &construc);
			DrawPolygonWire(spline, SplineColor->at(0), SplineColor->at(1), SplineColor->at(2));
			glBegin(GL_POINTS);
			glColor3f(1.0f, 1.0f, 1.0f);
			for (int i = 0; i < construc.size(); ++i)
			{
				glVertex2f(construc[i].x, construc[i].y);
			}
			glEnd();
			glBegin(GL_LINES);
			glColor3f(1.0f, 1.0f, 1.0f);
			for (int i = 0; i < construc.size() * 2 / 3; i += 2)
			{
				glVertex2f(construc[i].x, construc[i].y);
				glVertex2f(construc[i+1].x, construc[i+1].y);
			}
			glEnd();
		}
	}

	for (int i = 0; i < curves[2].size(); ++i)
	{
		DrawPolygonWire(curves[2][i], BaseColor->at(0), BaseColor->at(1), BaseColor->at(2));
		if (curves[2][i].size() > 2) {
			vector<point> bezier = DeCasteljau(curves[2][i]);
			DrawPolygonWire(bezier, DeCastelColor->at(0), DeCastelColor->at(1), DeCastelColor->at(2));
		}
		if (curves[2][i].size() > 4) {
			vector<point> construc;
			vector<point> spline = Spline(curves[2][i], &construc);
			DrawPolygonWire(spline, SplineColor->at(0), SplineColor->at(1), SplineColor->at(2));
			glBegin(GL_POINTS);
			glColor3f(1.0f, 1.0f, 1.0f);
			for (int i = 0; i < construc.size(); ++i)
			{
				glVertex2f(construc[i].x, construc[i].y);
			}
			glEnd();
			glBegin(GL_LINES);
			glColor3f(1.0f, 1.0f, 1.0f);
			for (int i = 0; i < construc.size() * 2 / 3; i += 2)
			{
				glVertex2f(construc[i].x, construc[i].y);
				glVertex2f(construc[i + 1].x, construc[i + 1].y);
			}
			glEnd();
		}
	}

	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(inter.x, inter.y);
	for (int i = 0; i < curves.size(); ++i)
	{
		for (int j = 0; j < curves[i].size(); ++j)
		{
			for (int k = 0; k < curves[i][j].size(); ++k)
			{
				glVertex2f(curves[i][j][k].x, curves[i][j][k].y);
			}
		}
	}
	glEnd();

	glutSwapBuffers();
	glFlush();
}


void DrawPolygonWire(vector<point> polygon, float r, float g, float b)
{
	glBegin(GL_LINES);
	glColor3f(r, g, b);
	int size = polygon.size() - 1;
	for (int i = 0; i < size; ++i){
		glVertex2f(polygon[i].x, polygon[i].y);
		glVertex2f(polygon[i + 1].x, polygon[i + 1].y);
	}
	glEnd();
}


void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN) {
			int current;
			switch (mode) {
			case 1:
				current = curves[0].size() - 1;
				curves[0][current].push_back(point(x, y));
				break;
			case 2:
				current = curves[1].size() - 1;
				curves[1][current].push_back(point(x, y));
				break;
			case 3:
				current = curves[2].size() - 1;
				curves[2][current].push_back(point(x, y));
				break;
			case 4:
				for (int i = 0; i < curves.size(); ++i) {
					for (int j = 0; j < curves[i].size(); ++j) {
						for (int k = 0; k < curves[i][j].size(); ++k) {
							if (curves[i][j][k].x - x < 15 && curves[i][j][k].x - x > -15 && curves[i][j][k].y - y < 15 && curves[i][j][k].y - y > -15) {
								pickedCurveType = i;
								pickedCurve = j;
								pickedPoint = k;
							}
						}
					}
				}
				break;
			case 5:
				pickedCurve = -1;
				for (int j = 0; j < curves[0].size(); ++j) {
					for (int k = 0; k < curves[0][j].size(); ++k) {
						if (curves[0][j][k].x - x < 15 && curves[0][j][k].x - x > -15 && curves[0][j][k].y - y < 15 && curves[0][j][k].y - y > -15) {
							pickedCurve = j;
						}
					}
				}
				if (pickedCurve != -1) {
					if (!raccordementStep) {
						newRaccordement->bezierA = pickedCurve;
						raccordementStep = true;
					}
					else {
						newRaccordement->bezierB = pickedCurve;
						newRaccordement->Init();
						raccordements.push_back(newRaccordement);
						mode = 0;
					}
				}
				break;
			case 6:
				for (int i = 0; i < curves.size(); ++i) {
					for (int j = 0; j < curves[i].size(); ++j) {
						for (int k = 0; k < curves[i][j].size(); ++k) {
							if (curves[i][j][k].x - x < 15 && curves[i][j][k].x - x > -15 && curves[i][j][k].y - y < 15 && curves[i][j][k].y - y > -15) {
								Translation(i, j);
								mode = 0;
							}
						}
					}
				}
				break;
			case 7:
				for (int i = 0; i < curves.size(); ++i) {
					for (int j = 0; j < curves[i].size(); ++j) {
						for (int k = 0; k < curves[i][j].size(); ++k) {
							if (curves[i][j][k].x - x < 15 && curves[i][j][k].x - x > -15 && curves[i][j][k].y - y < 15 && curves[i][j][k].y - y > -15) {
								Rotation(i, j);
								mode = 0;
							}
						}
					}
				}
				break;
			case 8:
				for (int i = 0; i < curves.size(); ++i) {
					for (int j = 0; j < curves[i].size(); ++j) {
						for (int k = 0; k < curves[i][j].size(); ++k) {
							if (curves[i][j][k].x - x < 15 && curves[i][j][k].x - x > -15 && curves[i][j][k].y - y < 15 && curves[i][j][k].y - y > -15) {
								Scaling(i, j);
								mode = 0;
							}
						}
					}
				}
				break;
			}
			glutPostRedisplay();
		}

		if (state == GLUT_UP)
		{
			switch (mode) {
			case 4:
				pickedCurveType = -1;
				pickedCurve = -1;
				pickedPoint = -1;
			}
		}
	}
}


void MouseMove(int x, int y)
{
	if (pickedCurveType > -1 && pickedCurve > -1 && pickedPoint > -1) {
		curves[pickedCurveType][pickedCurve][pickedPoint].x = x;
		curves[pickedCurveType][pickedCurve][pickedPoint].y = y;
		glutPostRedisplay();
	}
}


void KeyBoard(unsigned char touche, int x, int y){
	switch (touche)
	{
	case VK_ESCAPE:/* Quitter le programme */
		exit(0);
		break;

	case 'r':
		InitVariables();
		break;
	case '+':
		pas++;
		break;
	case '-':
		pas--;
		if (pas < 1)
			pas = 1;
		break;
	case '*':
		pasAdaptatif = !pasAdaptatif;
		break;
	}

	glutPostRedisplay();
}


#pragma endregion GLUT



#pragma region MENU


void AddMenu()
{
	int menuCourbes = glutCreateMenu(SelectCurves);
	glutAddMenuEntry("Bezier", 0);
	glutAddMenuEntry("Spline", 1);
	glutAddMenuEntry("Bezier/Spline", 2);

	int menuTransformations = glutCreateMenu(SelectTranformation);
	glutAddMenuEntry("Translation", 0);
	glutAddMenuEntry("Rotation", 1);
	glutAddMenuEntry("Scaling", 2);

	int menuColorBase = glutCreateMenu(SelectBaseColor);
	glutAddMenuEntry("Rouge", 0);
	glutAddMenuEntry("Vert", 1);
	glutAddMenuEntry("Bleu", 2);
	glutAddMenuEntry("Rose", 3);
	glutAddMenuEntry("Jaune", 4);
	glutAddMenuEntry("Violet", 5);
	glutAddMenuEntry("Orange", 6);
	int menuColorBeziers = glutCreateMenu(SelectDeCastelColor);
	glutAddMenuEntry("Rouge", 0);
	glutAddMenuEntry("Vert", 1);
	glutAddMenuEntry("Bleu", 2);
	glutAddMenuEntry("Rose", 3);
	glutAddMenuEntry("Jaune", 4);
	glutAddMenuEntry("Violet", 5);
	glutAddMenuEntry("Orange", 6);
	int menuColorSplines = glutCreateMenu(SelectSplineColor);
	glutAddMenuEntry("Rouge", 0);
	glutAddMenuEntry("Vert", 1);
	glutAddMenuEntry("Bleu", 2);
	glutAddMenuEntry("Rose", 3);
	glutAddMenuEntry("Jaune", 4);
	glutAddMenuEntry("Violet", 5);
	glutAddMenuEntry("Orange", 6);
	int menuColor = glutCreateMenu(SelectVoid);
	glutAddSubMenu("Points de contrôles", menuColorBase);
	glutAddSubMenu("Beziers", menuColorBeziers);
	glutAddSubMenu("Splines", menuColorSplines);

	int menuRaccordement = glutCreateMenu(SelectRaccordement);
	glutAddMenuEntry("C0", 0);
	glutAddMenuEntry("C1", 1);
	glutAddMenuEntry("C2", 2);

	glutCreateMenu(SelectMain);
	glutAddSubMenu("Courbes", menuCourbes);
	glutAddSubMenu("Transformations", menuTransformations);
	glutAddMenuEntry("Move Points", 3);
	glutAddSubMenu("Raccordement", menuRaccordement);
	glutAddSubMenu("Couleurs", menuColor);
	glutAddMenuEntry("Reset", 8);
	glutAddMenuEntry("Quitter", 7);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void SelectVoid(int i) {}

void SelectMain(int selection)
{
	ClearMode();
	switch (selection) {
	case 3:
		mode = 4;
		break;
	case 7:
		exit(0);
		break;
	case 8:
		InitVariables();
		break;
	}
	glutPostRedisplay();
}

void SelectCurves(int selection)
{
	ClearMode();
	switch (selection) {
	case 0:
		mode = 1;
		curves[0].push_back(vector<point>());
		break;
	case 1:
		mode = 2;
		curves[1].push_back(vector<point>());
		break;
	case 2:
		mode = 3;
		curves[2].push_back(vector<point>());
		break;
	}
}

void SelectTranformation(int selection)
{
	ClearMode();
	switch (selection) {
	case 0:
		mode = 6;
		break;
	case 1:
		mode = 7;
		break;
	case 2:
		mode = 8;
		break;
	}
}

vector<float>* SelectColor(int selection)
{
	vector<float>* v = new vector < float >();

	switch (selection)
	{
	case 0:
		v->push_back(1.0f);
		v->push_back(0.0f);
		v->push_back(0.0f);
		break;
	case 1:
		v->push_back(0.0f);
		v->push_back(1.0f);
		v->push_back(0.0f);
		break;
	case 2:
		v->push_back(0.0f);
		v->push_back(0.0f);
		v->push_back(1.0f);
		break;
	case 3:
		v->push_back(1.0f);
		v->push_back(0.2f);
		v->push_back(0.8f);
		break;
	case 4:
		v->push_back(1.0f);
		v->push_back(1.0f);
		v->push_back(0.0f);
		break;
	case 5:
		v->push_back(0.4f);
		v->push_back(0.0f);
		v->push_back(0.4f);
		break;
	case 6:
		v->push_back(1.0f);
		v->push_back(0.4f);
		v->push_back(0.0f);
		break;
	}

	return v;
}

void SelectBaseColor(int selection)
{
	ClearMode();
	BaseColor->clear();
	BaseColor = SelectColor(selection);
	glutPostRedisplay();
}

void SelectDeCastelColor(int selection)
{
	ClearMode();
	DeCastelColor->clear();
	DeCastelColor = SelectColor(selection);
	glutPostRedisplay();
}

void SelectSplineColor(int selection)
{
	ClearMode();
	SplineColor->clear();
	SplineColor = SelectColor(selection);
	glutPostRedisplay();
}

void SelectRaccordement(int selection)
{
	ClearMode();
	mode = 5;
	raccordementStep = false;
	switch (selection ) {
	case 0:
		newRaccordement = &C0s[nbC0++];
		break;
	case 1:
		newRaccordement = &C1s[nbC1++];
		break;
	case 2:
		newRaccordement = &C2s[nbC2++];
		break;
	}
}

void Translation(int curveType, int curve)
{
	float x, y = 0;
	printf("Entrer le facteur x : ");
	scanf_s("%f", &x);
	printf("Entrer le facteur y : ");
	scanf_s("%f", &y);
	Translation(curveType, curve, x, y);
}

void Translation(int curveType, int curve, float x, float y)
{
	vec4 direction(x, y, 0.0f, 1.0f);
	int size = curves[curveType][curve].size();
	for (int i = 0; i < size; i++)
	{
		curves[curveType][curve][i] = curves[curveType][curve][i] + direction;
	}
	glutPostRedisplay();
}

void Rotation(int curveType, int curve)
{
	static vec4 direction(0.0f, 0.0f, 0.0f, 1.0f);
	printf("Entrer le facteur z : ");
	scanf_s("%f", &(direction.z));

	point bary = BaryCentre(curves[curveType][curve]);
	Translation(curveType, curve, -bary.x, -bary.y);
	for (int i = 0; i < curves[curveType][curve].size(); i++)
	{
		curves[curveType][curve][i] = curves[curveType][curve][i] - direction;
	}
	Translation(curveType, curve, bary.x, bary.y);

	glutPostRedisplay();
}

void Scaling(int curveType, int curve)
{
	static vec4 direction(0.0f, 0.0f, 0.0f, 1.0f);
	printf("Entrer le facteur x : ");
	scanf_s("%f", &(direction.x));
	printf("Entrer le facteur y : ");
	scanf_s("%f", &(direction.y));

	point bary = BaryCentre(curves[curveType][curve]);
	Translation(curveType, curve, -bary.x, -bary.y);
	for (int i = 0; i < curves[curveType][curve].size(); i++)
	{
		curves[curveType][curve][i] = curves[curveType][curve][i] * direction;
	}
	Translation(curveType, curve, bary.x, bary.y);

	glutPostRedisplay();
}


point BaryCentre(vector<point> points)
{
	float x = 0, y = 0;
	for (int i = 0; i < points.size(); ++i)
	{
		x += points[i].x;
		y += points[i].y;
	}
	x /= points.size();
	y /= points.size();
	return point(x, y);
}


bool Intersection(point a, point b, point c, point d, point* i)
{
	//matrice 1, matrice inverse.
	float matrice[2][2];
	float matriceInverse[2][2];
	float matriceRes[2];
	float matriceB[2];

	float determinant;

	matrice[0][0] = (b.x - a.x);
	matrice[0][1] = (c.x - d.x);
	matrice[1][0] = (b.y - a.y);
	matrice[1][1] = (c.y - d.y);

	determinant = (matrice[0][0] * matrice[1][1]) - (matrice[0][1] * matrice[1][0]);

	if (determinant == 0)
		return false;
	
	matriceInverse[0][0] = matrice[1][1] / determinant;
	matriceInverse[0][1] = -matrice[0][1] / determinant;
	matriceInverse[1][0] = -matrice[1][0] / determinant;
	matriceInverse[1][1] = matrice[0][0] / determinant;

	matriceB[0] = (c.x - a.x);
	matriceB[1] = (c.y - a.y);

	matriceRes[0] = matriceInverse[0][0] * matriceB[0] + matriceInverse[0][1] * matriceB[1];
	matriceRes[1] = matriceInverse[1][0] * matriceB[0] + matriceInverse[1][1] * matriceB[1];

	i->x = ((1 - matriceRes[0]) * a.x) + (matriceRes[0] * b.x);
	i->y = ((1 - matriceRes[0]) * a.y) + (matriceRes[0] * b.y);

	return true;
}


void ClearMode()
{
	switch (mode)
	{
	case 1:
		if (curves[0][curves[0].size() - 1].size() < 3)
			curves[0].pop_back();
		break;
	case 2:
		if (curves[1][curves[1].size() - 1].size() < 3)
			curves[1].pop_back();
		break;
	case 3:
		if (curves[2][curves[2].size() - 1].size() < 3)
			curves[2].pop_back();
		break;
	case 4:
		pickedCurveType = -1;
		pickedCurve = -1;
		pickedPoint = -1;
		break;
	}
	mode = 0;
	glutPostRedisplay();
}


#pragma endregion MENU


vector<point> DeCasteljau(vector<point> polygon)
{
	vector<point> _bezier;
	float p;
	if (pasAdaptatif) {
		float f = 0;
		for (int i = 0; i < polygon.size() - 1; ++i) {
			f += Distance(polygon[i], polygon[i + 1]);
		}
		p = f / 100;
	}
	else
		p = pas;

	point _point;
	for (int k = 0; k <= p; ++k) {
		_point = GetBezierPoint(k / p, polygon);
		_bezier.push_back(_point);
	}

	return _bezier;
}


point GetBezierPoint(float t, vector<point> polygon)
{
	int n = polygon.size() - 1;

	vector<vector<point>> P;
	P.push_back(vector<point>());
	for (int i = 0; i <= n; ++i) {
		P[0].push_back(polygon[i]);
	}

	point _point;
	for (int i = 1; i <= n; ++i) {
		P.push_back(vector<point>());
		for (int j = 0; j <= n - i; ++j) {
			_point = P[i - 1][j] * (1 - t) + P[i - 1][j + 1] * t;
			P[i].push_back(_point);
		}
	}

	return P[n][0];
}


float Distance(point a, point b)
{
	point p = a - b;
	return sqrt(p.x*p.x + p.y*p.y);
}


vector<point> Spline(vector<point> polygon, vector<point>* construc)
{
	int size = polygon.size();
	if (size < 5)
		return DeCasteljau(polygon);

	vector<point> _spline;
	vector<point> R;
	vector<point> U;
	vector<vector<point>> P;

	R.push_back((polygon[1] + polygon[2]) * 0.5f);
	for (int i = 2; i < size - 3; ++i) {
		R.push_back((polygon[i] * 2 + polygon[i+1]) * 0.333f);
		R.push_back((polygon[i] + polygon[i+1] * 2) * 0.333f);
	}
	R.push_back((polygon[size-3] + polygon[size-2]) * 0.5f);

	for (int i = 0; i < R.size(); i += 2) {
		U.push_back((R[i] + R[i+1]) * 0.5f);
	}

	P.push_back(vector<point>());
	P[0].push_back(polygon[0]);
	P[0].push_back(polygon[1]);
	P[0].push_back(R[0]);
	P[0].push_back(U[0]);
	for (int i = 1; i < size - 4; ++i) {
		P.push_back(vector<point>());
		P[i].push_back(U[i-1]);
		P[i].push_back(R[i*2-1]);
		P[i].push_back(R[i*2]);
		P[i].push_back(U[i]);
	}
	P.push_back(vector<point>());
	P[size-4].push_back(U[U.size()-1]);
	P[size-4].push_back(R[R.size()-1]);
	P[size-4].push_back(polygon[polygon.size()-2]);
	P[size-4].push_back(polygon[polygon.size()-1]);
	
	for (int i = 0; i < P.size(); ++i) {
		vector<point> _bezier;
		_bezier = DeCasteljau(P[i]);
		for (int j = 0; j < _bezier.size(); ++j) {
			_spline.push_back(_bezier[j]);
		}
	}

	for (int i = 0; i < R.size(); ++i) {
		construc->push_back(R[i]);
	}

	for (int i = 0; i < U.size(); ++i) {
		construc->push_back(U[i]);
	}

	return _spline;
}

void InitVariables()
{
	mode = 0;

	pickedCurveType = -1;
	pickedCurve = -1;
	pickedPoint = -1;
	pasAdaptatif = false;
	pas = 10;
	raccordementStep = false;

	curves.clear();
	curves.push_back(vector<vector<point>>());
	curves.push_back(vector<vector<point>>());
	curves.push_back(vector<vector<point>>());

	BaseColor->clear();
	BaseColor->push_back(0.0f);
	BaseColor->push_back(0.0f);
	BaseColor->push_back(1.0f);

	DeCastelColor->clear();
	DeCastelColor->push_back(1.0f);
	DeCastelColor->push_back(0.0f);
	DeCastelColor->push_back(0.0f);

	SplineColor->clear();
	SplineColor->push_back(0.0f);
	SplineColor->push_back(1.0f);
	SplineColor->push_back(0.0f);

	nbC0 = 0;
	nbC1 = 0;
	nbC2 = 0;
	raccordements.clear();
	inter = point(-5, -5);
}
