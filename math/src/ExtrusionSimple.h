#pragma once

#include "ofMain.h"
#include "BSpline.h"

class ExtrusionSimple
{
public:
	ExtrusionSimple(void);
	~ExtrusionSimple(void);

	void Draw();

	void Extrude();

	BSpline bSplineBase;

	int nbPas;
	float height;
	float sizeRatio;

	vector<vector<ofVec3f>> extrusionVertices;
};

