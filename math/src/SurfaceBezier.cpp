#include "SurfaceBezier.h"


SurfaceBezier::SurfaceBezier(void)
{
}


SurfaceBezier::SurfaceBezier(int _nbControls)
{
	if (_nbControls < 3) _nbControls = 3;
	nbControls = _nbControls;
	float offset = 500.0f / (nbControls - 1);
	for (int i = 0; i < nbControls; ++i) {
		controlPoints.push_back(vector<ofVec3f>());
		for (int j = 0; j < nbControls; ++j) {
			controlPoints[i].push_back(ofVec3f(-250 + i * offset, 0, 250 + j * -offset));
		}
	}

	nbPas = 20;
	GenerateBezierSurface();
}


SurfaceBezier::~SurfaceBezier(void)
{
}


void SurfaceBezier::Draw()
{
	ofSetColor(ofColor::red);
	for (int i = 0; i < nbPas; ++i) {
		for (int j = 0; j <= nbPas; ++j) {
			ofLine(
				bezierSurfaceVertices[i][j],
				bezierSurfaceVertices[i+1][j] );
		}
	}
	for (int i = 0; i <= nbPas; ++i) {
		for (int j = 0; j < nbPas; ++j) {
			ofLine(
				bezierSurfaceVertices[i][j],
				bezierSurfaceVertices[i][j+1] );
		}
	}

	ofSetColor(ofColor::black);
	for (int i = 0; i < nbControls - 1; ++i) {
		for (int j = 0; j < nbControls; ++j) {
			ofLine(
				controlPoints[i][j],
				controlPoints[i+1][j] );
		}
	}
	for (int i = 0; i < nbControls; ++i) {
		for (int j = 0; j < nbControls - 1; ++j) {
			ofLine(
				controlPoints[i][j],
				controlPoints[i][j+1] );
		}
	}
}


void SurfaceBezier::GenerateBezierSurface()
{
	bezierSurfaceVertices.clear();
	for (int i = 0; i <= nbPas; ++i) {
		bezierSurfaceVertices.push_back(vector<ofVec3f>());
		vector<ofVec3f> controlsTmp;
		for (int k = 0; k < nbControls; ++k) {
			controlsTmp.push_back(DeCasteljau(float(i)/nbPas, controlPoints[k]));
		}
		for (int j = 0; j <= nbPas; ++j) {
			ofVec3f point = DeCasteljau(float(j)/nbPas, controlsTmp);
			bezierSurfaceVertices[i].push_back(point);
		}
	}
}


ofVec3f SurfaceBezier::DeCasteljau(float t, vector<ofVec3f> ctrlPoints)
{
	int n = nbControls - 1;

	vector<vector<ofVec3f>> P;
	P.push_back(vector<ofVec3f>());
	for (int i = 0; i <= n; ++i) {
		P[0].push_back(ctrlPoints[i]);
	}

	ofVec3f point;
	for (int i = 1; i <= n; ++i) {
		P.push_back(vector<ofVec3f>());
		for (int j = 0; j <= n - i; ++j) {
			point = P[i-1][j] * (1-t) + P[i-1][j+1] * t;
			P[i].push_back(point);
		}
	}
	
	return P[n][0];
}

