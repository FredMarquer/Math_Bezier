#pragma once

#include "ofMain.h"

class BSpline
{
public:
	BSpline(void);
	~BSpline(void);

	void Draw();

	void GenerateBSpline();
	ofVec3f CalculateBSplinePoint(float t, vector<float> T, vector<ofVec3f> controls);
	float BSpline::DeBoor(int p, int i, float t, vector<float> knots);

	vector<ofVec3f> controlPoints;
	vector<float> knots;

	int nbPas;
	vector<ofVec3f> bSplinePoints;
};