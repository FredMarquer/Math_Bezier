#include "ExtrusionRevolution.h"


ExtrusionRevolution::ExtrusionRevolution(void)
{
	angle = 360;
	nbPas = 36;

	bSplineBase.controlPoints.push_back(ofVec3f(0, -200, 0));
	bSplineBase.controlPoints.push_back(ofVec3f(300, -100, 0));
	bSplineBase.controlPoints.push_back(ofVec3f(100, 0, 0));
	bSplineBase.controlPoints.push_back(ofVec3f(400, 100, 0));
	bSplineBase.controlPoints.push_back(ofVec3f(100, 200, 0));
	bSplineBase.nbPas = 20;
	bSplineBase.CheckKnots();
	bSplineBase.GenerateBSpline();
	Extrude();
}


ExtrusionRevolution::~ExtrusionRevolution(void)
{
}


void ExtrusionRevolution::Draw()
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


void ExtrusionRevolution::Extrude()
{
	extrusionVertices.clear();
	for (int i = 0; i <= nbPas; ++i) {
		extrusionVertices.push_back(vector<ofVec3f>());
		float currentAngle = angle * i / nbPas;
		for (int j = 0; j < bSplineBase.bSplinePoints.size(); ++j) {
			ofVec3f point = bSplineBase.bSplinePoints[j];
			point.rotate(0, currentAngle, 0);
			extrusionVertices[i].push_back(point);
		}
	}
}
