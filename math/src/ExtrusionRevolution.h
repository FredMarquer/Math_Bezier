#pragma once

#include "ofMain.h"
#include "BSpline.h"

class ExtrusionRevolution
{
public:
	ExtrusionRevolution(void);
	~ExtrusionRevolution(void);

	void Draw();

	void Extrude();

	BSpline bSplineBase;

	float angle;
	int nbPas;

	vector<vector<ofVec3f>> extrusionVertices;
};

