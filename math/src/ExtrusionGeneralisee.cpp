#include "ExtrusionGeneralisee.h"


ExtrusionGeneralisee::ExtrusionGeneralisee(void)
{
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