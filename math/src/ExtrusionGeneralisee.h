#pragma once

#include "ofMain.h"
#include "BSpline.h"

class ExtrusionGeneralisee
{
public:
	ExtrusionGeneralisee(void);
	~ExtrusionGeneralisee(void);

	void Draw();

	void Extrude();

	BSpline bSplineBase;
	BSpline bSplineGuide;

	vector<vector<ofVec3f>> extrusionVertices;
};

