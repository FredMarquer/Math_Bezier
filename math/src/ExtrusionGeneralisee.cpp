#include "ExtrusionGeneralisee.h"


ExtrusionGeneralisee::ExtrusionGeneralisee(void)
{
	bSplineBase.controlPoints.push_back(ofVec3f(-200, 0, -200));
	bSplineBase.controlPoints.push_back(ofVec3f(-200, 0, 100));
	bSplineBase.controlPoints.push_back(ofVec3f(100, 0, 100));
	bSplineBase.controlPoints.push_back(ofVec3f(200, 0, -100));
	bSplineBase.controlPoints.push_back(ofVec3f(200, 0, 200));
	bSplineBase.nbPas = 20;
	bSplineBase.CheckKnots();
	bSplineBase.GenerateBSpline();

	bSplineGuide.controlPoints.push_back(ofVec3f(0, -200, 0));
	bSplineGuide.controlPoints.push_back(ofVec3f(300, -100, 0));
	bSplineGuide.controlPoints.push_back(ofVec3f(100, 0, 0));
	bSplineGuide.controlPoints.push_back(ofVec3f(400, 100, 0));
	bSplineGuide.controlPoints.push_back(ofVec3f(100, 200, 0));
	bSplineGuide.nbPas = 20;
	bSplineGuide.CheckKnots();
	bSplineGuide.GenerateBSpline();

	Extrude();
}


ExtrusionGeneralisee::~ExtrusionGeneralisee(void)
{
}


void ExtrusionGeneralisee::Draw()
{
	ofSetColor(ofColor::red);
	for (int i = 0; i < bSplineBase.nbPas; ++i) {
		for (int j = 0; j <= bSplineGuide.nbPas; ++j) {
			ofLine(
				extrusionVertices[i][j],
				extrusionVertices[i+1][j] );
		}
	}
	for (int i = 0; i <= bSplineBase.nbPas; ++i) {
		for (int j = 0; j < bSplineGuide.nbPas; ++j) {
			ofLine(
				extrusionVertices[i][j],
				extrusionVertices[i][j+1] );
		}
	}
}


void ExtrusionGeneralisee::Extrude()
{
	extrusionVertices.clear();
	ofVec3f offset = bSplineGuide.bSplinePoints[0];
	for (int i = 0; i < bSplineBase.bSplinePoints.size(); ++i) {
		extrusionVertices.push_back(vector<ofVec3f>());
		for (int j = 0; j < bSplineGuide.bSplinePoints.size(); ++j) {
			ofVec3f point = bSplineBase.bSplinePoints[i] + bSplineGuide.bSplinePoints[j] - offset;
			extrusionVertices[i].push_back(point);
		}
	}
}