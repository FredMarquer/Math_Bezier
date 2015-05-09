



#pragma region DECLARATIONS

#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "stdafx.h"
#include <iostream>
#include <vector> //Ne pas oublier !
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

	point(){};
	point(float _x, float _y) : x(_x), y(_y)
	{
	}
};

struct vec4
{
	float x, y, z, w;

	vec4(){};
	vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
	{
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

	m.m00 = cosf(d.x * PI / 180.0);
	m.m01 = -sinf(d.x * PI / 180.0);
	m.m10 = sinf(d.x * PI / 180.0);
	m.m11 = cosf(d.x * PI / 180.0);

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

// Polygons variables
vector<point> polygon;

// Window variables
int windowWidth = 500;
int windowHeight = 500;

//Matrice rotation
bool rotated = false;
vec4 directionRotation(0.0f, 0.0f, 0.0f, 1.0f);
vector<point>* rotationPointsOrigin;

float _xMin = windowWidth;
float _yMin = windowHeight;




// States variables
int pickedPoint = -1;
float pasAdaptif = 25;


// Prototypes
void Display(void);
void DrawPolygonWire(vector<point> polygon, float r, float g, float b);
void Keyboard(unsigned char touche, int x, int y);
void Mouse(int bouton, int etat, int x, int y); 
void MouseMove(int x, int y);
void AddMenu();
void AssignColor();
void Select(int selection);

vector<float>* SelectColor(int selection);

void SelectBaseColor(int selection);
void SelectDeCastelColor(int selection);
void SelectSplineColor(int selection);
void SelectParamUtil(int selection);
void SelectMatriceScaling(int selection);
void SelectMatriceRotation(int selection);
void AjouterPosition();

void Reset();

vector<point> DeCasteljau(vector<point>);
point GetBezierPoint(float f, vector<point> polygon);
vector<point> Spline(vector<point> polygon);

vector<float>* BaseColor = new vector<float>;
vector<float>* DeCastelColor = new vector<float>;
vector<float>* SplineColor = new vector<float>;
#pragma endregion DECLARATIONS

#pragma region POINT_VEC_MAT



#pragma endregion POINT_VEC_MAT

#pragma region MAIN


int main(int argc, char **argv)
{  
	// Initialisation de glut et creation de la fenetre 
	glutInit(&argc,argv);                       // Initialisation
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); // mode d'affichage RGB, et test prafondeur
	glutInitWindowSize(windowWidth, windowHeight);                // dimension fenêtre
	glutInitWindowPosition(100, 100);           // position coin haut gauche
	glutCreateWindow("A vous de jouer!!!");  // nom

	// Repère 2D délimitant les abscisses et les ordonnées
	gluOrtho2D(0, windowWidth, windowHeight, 0);

	// Initialisation d'OpenGL
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glPointSize(1.0);               // taille d'un point: 1px

	// Enregistrement des fonctions de rappel
	// => initialisation des fonctions callback appelées par glut 
	//glutIdleFunc(afficher);
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMove);
	// rq: le callback de fonction (fonction de rappel) est une fonction qui est passée en argument à une
	// autre fonction. Ici, le main fait usage des deux fonctions de rappel (qui fonctionnent en même temps)
	// alors qu'il ne les connaît pas par avance.
	AssignColor();

	AddMenu();


	// Entrée dans la boucle principale de glut, traitement des évènements 
	glutMainLoop();         // lancement de la boucle de réception des évènements
	system("pause");

	return 0;
}


#pragma endregion MAIN



#pragma region GLUT


void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	DrawPolygonWire(polygon, BaseColor->at(0), BaseColor->at(1), BaseColor->at(2));
	
	if (polygon.size() > 2) {
		vector<point> bezier = DeCasteljau(polygon);
		DrawPolygonWire(bezier, DeCastelColor->at(0), DeCastelColor->at(1), DeCastelColor->at(2));
		vector<point> spline = Spline(polygon);
		DrawPolygonWire(spline, SplineColor->at(0), SplineColor->at(1), SplineColor->at(2));
	}
	
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
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		polygon.push_back(point(x, y));

		glutPostRedisplay();
	}

	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN) {
			for (int i = 0; i < polygon.size(); ++i) {
				if (polygon[i].x - x < 15 && polygon[i].x - x > -15 && polygon[i].y - y < 15 && polygon[i].y - y > -15)
					pickedPoint = i;
			}
		}

		if (state == GLUT_UP)
			pickedPoint = -1;
	}
}


void MouseMove(int x, int y)
{
	if (pickedPoint > -1) {
		polygon[pickedPoint].x = x;
		polygon[pickedPoint].y = y;
		glutPostRedisplay();
	}
}


void Keyboard(unsigned char touche, int x, int y){
	switch (touche)
	{
	case VK_ESCAPE:/* Quitter le programme */
		exit(0);
		break;

	case 'r':
		Reset();
		break;
	case '+':
		pasAdaptif++;
		break;
	case '-':
		pasAdaptif--;
		break;
	}

	


	glutPostRedisplay();
}


#pragma endregion GLUT



#pragma region MENU


void Reset()
{
	polygon.clear();
}

void AddMenu()
{
	int menuBColorWindow = glutCreateMenu(SelectBaseColor);
	glutAddMenuEntry("Rouge", 0);
	glutAddMenuEntry("Vert", 1);
	glutAddMenuEntry("Bleu", 2);
	glutAddMenuEntry("Rose", 3);
	glutAddMenuEntry("Jaune", 4);
	glutAddMenuEntry("Violet", 5);
	glutAddMenuEntry("Orange", 6);
	int menuDCWindow = glutCreateMenu(SelectDeCastelColor);
	glutAddMenuEntry("Rouge", 0);
	glutAddMenuEntry("Vert", 1);
	glutAddMenuEntry("Bleu", 2);
	glutAddMenuEntry("Rose", 3);
	glutAddMenuEntry("Jaune", 4);
	glutAddMenuEntry("Violet", 5);
	glutAddMenuEntry("Orange", 6);
	int menuSPWindow = glutCreateMenu(SelectSplineColor);
	glutAddMenuEntry("Rouge", 0);
	glutAddMenuEntry("Vert", 1);
	glutAddMenuEntry("Bleu", 2);
	glutAddMenuEntry("Rose", 3);
	glutAddMenuEntry("Jaune", 4);
	glutAddMenuEntry("Violet", 5);
	glutAddMenuEntry("Orange", 6);
	int menuTranslationWindow = glutCreateMenu(SelectParamUtil);
	glutAddMenuEntry("X", 0);
	glutAddMenuEntry("Y", 1);
	int menuScalingWindow = glutCreateMenu(SelectMatriceScaling);
	glutAddMenuEntry("X", 0);
	glutAddMenuEntry("Y", 1);
	int menuRotationWindow = glutCreateMenu(SelectMatriceRotation);
	glutAddMenuEntry("X", 0);
	glutAddMenuEntry("Y", 1);
	glutCreateMenu(Select);
	glutAddSubMenu("Couleur point de passage", menuBColorWindow);
	glutAddSubMenu("Couleur De casteljau", menuDCWindow);
	glutAddSubMenu("Couleur Spline", menuSPWindow);
	glutAddSubMenu("Matrice Translation", menuTranslationWindow);
	glutAddSubMenu("Matrice Scaling", menuScalingWindow);
	glutAddSubMenu("Matrice Rotation", menuRotationWindow);
	glutAddMenuEntry("Ajouter Point", 9);
	glutAddMenuEntry("Reset", 8);
	glutAddMenuEntry("Quitter", 7);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void Select(int selection)
{
	switch (selection) {
	case 7:
		exit(0);
		break;
	case 8:
		Reset();
		break;
	case 9:
		AjouterPosition();
		break;
	}
	glutPostRedisplay();
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

void SelectBaseColor(int selection) {

	BaseColor->clear();
	BaseColor = SelectColor(selection);

	glutPostRedisplay();
}

void SelectDeCastelColor(int selection)
{
	DeCastelColor->clear();
	DeCastelColor = SelectColor(selection);

	glutPostRedisplay();
}

void SelectSplineColor(int selection)
{
	SplineColor->clear();
	SplineColor = SelectColor(selection);

	glutPostRedisplay();
}

void SelectParamUtil(int selection)
{
	vec4 direction(0.0f,0.0f,0.0f,1.0f);
	
	int size = polygon.size();

	switch (selection)
	{
	case 0:
		printf("Entrer le facteur x : ");
		scanf_s("%f", &(direction.x));
		for (int i = 0; i < size; i++)
		{
			polygon[i] = polygon[i] + direction;
		}
		break;
	case 1:
		printf("Entrer le facteur y : ");
		scanf_s("%f", &(direction.y));
		for (int i = 0; i < size; i++)
		{
			polygon[i] = polygon[i] + direction;
		}
		break;
	}

	glutPostRedisplay();
}

void SelectMatriceScaling(int selection) {
	vec4 direction(0.0f, 0.0f, 0.0f, 1.0f);

	int size = polygon.size();

	vector<point> pointsMin_Max;
	for (int i = 0; i < size; i++)
	{
		if (!rotated)
		{
			if (_xMin > polygon.at(i).x)
			{
				_xMin = polygon.at(i).x;
			}
			if (_yMin > polygon.at(i).y)
			{
				_yMin = polygon.at(i).y;
			}
		}
		
		pointsMin_Max.push_back(polygon.at(i));
	}

	for (int i = 0; i < size; i++)
	{
		pointsMin_Max.at(i).x -= _xMin;
		pointsMin_Max.at(i).y -= _yMin;
	}

	switch (selection)
	{
	case 0:
		printf("Entrer le facteur x : ");
		scanf_s("%f", &(direction.x));
		for (int i = 0; i < size; i++)
		{
			pointsMin_Max[i] = pointsMin_Max[i] * direction;
		}
		break;
	case 1:
		printf("Entrer le facteur y : ");
		scanf_s("%f", &(direction.y));
		for (int i = 0; i < size; i++)
		{
			pointsMin_Max[i] = pointsMin_Max[i] * direction;
		}
		break;
	}

	polygon.clear();

	for (int i = 0; i < size; i++)
	{
		pointsMin_Max.at(i).x += _xMin;
		pointsMin_Max.at(i).y += _yMin;
		polygon.push_back(pointsMin_Max.at(i));
	}

	rotated = true;

	glutPostRedisplay();
}

void SelectMatriceRotation(int selection)
{
	static vec4 directionRotation(0.0f, 0.0f, 0.0f, 1.0f);

	int size = polygon.size();

	

	vector<point> pointsMin_Max;
	for (int i = 0; i < size; i++)
	{
		if (!rotated)
		{
			if (_xMin > polygon.at(i).x)
			{
				_xMin = polygon.at(i).x;
			}
			if (_yMin > polygon.at(i).y)
			{
				_yMin = polygon.at(i).y;
			}
		}
		
		pointsMin_Max.push_back(polygon.at(i));
	}

	for (int i = 0; i < size; i++)
	{
		pointsMin_Max.at(i).x -= _xMin;
		pointsMin_Max.at(i).y -= _yMin;
	}
	
	switch (selection)
	{
	case 0:
		printf("Entrer le facteur x : ");
		scanf_s("%f", &(directionRotation.x));
		for (int i = 0; i < size; i++)
		{
			pointsMin_Max[i] = pointsMin_Max[i] - directionRotation;
		}
		break;
	case 1:
		printf("Entrer le facteur x : ");
		scanf_s("%f", &(directionRotation.x));
		for (int i = 0; i < size; i++)
		{
			pointsMin_Max[i] = pointsMin_Max[i] - directionRotation;
		}
		break;
	}

	polygon.clear();

	for (int i = 0; i < size; i++)
	{
		pointsMin_Max.at(i).x += _xMin;
		pointsMin_Max.at(i).y += _yMin;
		polygon.push_back(pointsMin_Max.at(i));
	}
	rotated = true;

	glutPostRedisplay();
}

void AjouterPosition()
{
	float _x;
	float _y;

	printf("Entrer le facteur x : ");
	scanf_s("%f", &(_x));
	printf("Entrer le facteur y : ");
	scanf_s("%f", &(_y));

	polygon.push_back(point(_x, _y));

	glutPostRedisplay();
}
#pragma endregion MENU


vector<point> DeCasteljau(vector<point> polygon)
{
	vector<point> _bezier;
	float p = pasAdaptif;

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


vector<point> Spline(vector<point> polygon)
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

	return _spline;
}

void AssignColor()
{
	BaseColor->push_back(0.0f);
	BaseColor->push_back(0.0f);
	BaseColor->push_back(1.0f);

	DeCastelColor->push_back(1.0f);
	DeCastelColor->push_back(0.0f);
	DeCastelColor->push_back(0.0f);

	SplineColor->push_back(0.0f);
	SplineColor->push_back(1.0f);
	SplineColor->push_back(0.0f);
}
