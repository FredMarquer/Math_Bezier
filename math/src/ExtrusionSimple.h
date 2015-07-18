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

	float height;
	float sizeRatio;
	int nbPas;

	vector<vector<ofVec3f>> extrusionVertices;
};

