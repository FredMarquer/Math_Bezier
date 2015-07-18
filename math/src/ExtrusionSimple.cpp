#include "ExtrusionSimple.h"


ExtrusionSimple::ExtrusionSimple(void)
{
	nbPas = 20;
	height = 100;
	sizeRatio = 1;
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

	bSplineBase.Draw();
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