#pragma once

#include "ofMain.h"

class BSpline
{
public:
	BSpline(void);
	~BSpline(void);

	void Draw();
	void DrawPoints();

	void GenerateBSpline();
	ofVec3f CalculateBSplinePoint(float t, vector<float> T, vector<ofVec3f> controls);
	float BSpline::DeBoor(int p, int i, float t, vector<float> knots);
	int GetControlPointAtPosition(ofVec2f position);
	void UpdateDegree(int newDegree);
	void CheckKnots();
	void MoveControl(int controlIndex, ofVec3f pos);
	void UpdateLocalBSpline(int controlIndex);

	vector<ofVec3f> controlPoints;
	vector<float> knots;

	int degree;
	int nbPas;
	vector<ofVec3f> bSplinePoints;
};