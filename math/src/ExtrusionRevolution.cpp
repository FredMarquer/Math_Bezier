#include "ExtrusionRevolution.h"


ExtrusionRevolution::ExtrusionRevolution(void)
{
	angle = 360;
	nbPas = 36;
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

	bSplineBase.Draw();
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
