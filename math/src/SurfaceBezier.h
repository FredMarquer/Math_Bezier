#pragma once

#include "ofMain.h"

class SurfaceBezier
{
public:
	SurfaceBezier(void);
	SurfaceBezier(int nbControls);
	~SurfaceBezier(void);

	void Draw();

	void GenerateBezierSurface();
	ofVec3f DeCasteljau(float t, vector<ofVec3f> ctrlPoints);

	int nbControls;
	vector<vector<ofVec3f>> controlPoints;

	int nbPas;
	vector<vector<ofVec3f>> bezierSurfaceVertices;
};

