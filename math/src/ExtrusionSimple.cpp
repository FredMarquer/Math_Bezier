#include "ExtrusionSimple.h"


ExtrusionSimple::ExtrusionSimple(void)
{
	height = 200;
	sizeRatio = 1.5f;
	nbPas = 20;

	bSplineBase.controlPoints.push_back(ofVec3f(-200, 0, -200));
	bSplineBase.controlPoints.push_back(ofVec3f(-200, 0, 100));
	bSplineBase.controlPoints.push_back(ofVec3f(100, 0, 100));
	bSplineBase.controlPoints.push_back(ofVec3f(200, 0, -100));
	bSplineBase.controlPoints.push_back(ofVec3f(200, 0, 200));
	bSplineBase.nbPas = 20;
	bSplineBase.CheckKnots();
	bSplineBase.GenerateBSpline();
	Extrude();
}


ExtrusionSimple::~ExtrusionSimple(void)
{
}


void ExtrusionSimple::Draw()
{
	ofSetColor(ofColor::red);
	for (int i = 0; i < nbPas; ++i) {
		for (int j = 0; j <= bSplineBase.nbPas; ++j) {
			ofLine(
				extrusionVertices[i][j],
				extrusionVertices[i+1][j] );
		}
	}
	for (int i = 0; i <= nbPas; ++i) {
		for (int j = 0; j < bSplineBase.nbPas; ++j) {
			ofLine(
				extrusionVertices[i][j],
				extrusionVertices[i][j+1] );
		}
	}
}


void ExtrusionSimple::Extrude()
{
	extrusionVertices.clear();
	for (int i = 0; i <= nbPas; ++i) {
		extrusionVertices.push_back(vector<ofVec3f>());
		float currentHeight = height * i / nbPas;
		float currentRatioSize = (sizeRatio - 1) * i / nbPas + 1;
		for (int j = 0; j < bSplineBase.bSplinePoints.size(); ++j) {
			ofVec3f point = bSplineBase.bSplinePoints[j];
			point *= currentRatioSize;
			point.y = currentHeight;
			extrusionVertices[i].push_back(point);
		}
	}
}