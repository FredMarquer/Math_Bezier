

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
using namespace std;


// Struct
struct point
{
	float x, y;

	point(){};
	point(float _x, float _y) : x(_x), y(_y)
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


// Window variables
int windowWidth = 500;
int windowHeight = 500;


// States variables
int pickedPoint = -1;


// Polygons variables
vector<point> polygon;


// Prototypes
void Display(void);
void DrawPolygonWire(vector<point> polygon, float r, float g, float b);
void Keyboard(unsigned char touche, int x, int y);
void Mouse(int bouton, int etat, int x, int y); 
void MouseMove(int x, int y);

void Reset();

vector<point> DeCasteljau(vector<point>);
point GetBezierPoint(float f, vector<point> polygon);
vector<point> Spline(vector<point> polygon);


#pragma endregion DECLARATIONS



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

	DrawPolygonWire(polygon, 0.0, 0.0, 1.0);
	
	if (polygon.size() > 2) {
		vector<point> bezier = DeCasteljau(polygon);
		DrawPolygonWire(bezier, 1.0, 0.0, 0.0);
		vector<point> spline = Spline(polygon);
		DrawPolygonWire(spline, 0.0, 1.0, 0.0);
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
	}

	glutPostRedisplay();
}


#pragma endregion GLUT



#pragma region MENU


void Reset()
{
	polygon.clear();
}


#pragma endregion MENU


vector<point> DeCasteljau(vector<point> polygon)
{
	vector<point> _bezier;
	float p = 25;

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