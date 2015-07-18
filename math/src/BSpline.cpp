#include "BSpline.h"


BSpline::BSpline(void)
{
	nbPas = 50;
	degree = 3;
}


BSpline::~BSpline(void)
{
}


void BSpline::Draw()
{
	ofSetColor(ofColor::red);
	int imax  = bSplinePoints.size() - 1;
	for (int i = 0; i < imax; ++i) {
		ofLine(bSplinePoints[i], bSplinePoints[i+1]);
	}

	ofSetColor(ofColor::black);
	imax = controlPoints.size() - 1;
	for (int i = 0; i < imax; ++i) 
		ofLine(controlPoints[i], controlPoints[i+1]);
}


void BSpline::DrawPoints()
{
	for (int i = 0; i < controlPoints.size(); ++i) {
		ofPushMatrix();
		ofTranslate(controlPoints[i]);
		ofFill();
		ofSetColor(ofColor::white);
		ofDrawBox(8);
		ofNoFill();
		ofSetColor(ofColor::black);
		ofDrawBox(8);
		ofPopMatrix();
	}
}


void BSpline::GenerateBSpline()
{
	bSplinePoints.clear();

	if (controlPoints.size() < 3)
		return;
	
	for (int i = 0; i <= nbPas; ++i) {
		float t = float(i)/nbPas * (knots[knots.size()-degree-1] - knots[degree]) + knots[degree];
		ofVec3f point = CalculateBSplinePoint(t, knots, controlPoints);
		bSplinePoints.push_back(point);
	}
}


ofVec3f BSpline::CalculateBSplinePoint(float t, vector<float> T, vector<ofVec3f> controls)
{
	ofVec3f point = ofVec3f(0, 0, 0);
	for (int i = 0; i < controls.size(); ++i) {
		point += controls[i] * DeBoor(degree, i, t, T);
	}
	return point;
}

/*
float BSpline::DeBoor(int p, int i, float t, vector<float> T)
{
    if (p == 0) {
		if (t >= T[i] && t < T[i+1])
			return 1;
		else
			return 0;
	}
    else {
		float a, b, c, d;

		a = T[i+p] - T[i];
		if (a == 0)
			b = 0;
		else
			b = DeBoor(p-1, i, t, T) * float(t - T[i]) / a;

		c = T[i+1+p] - T[i+1];
		if (c == 0)
			d = 0;
		else
			d = DeBoor(p-1, i+1, t, T) * float(T[i+1+p] - t) / c;
		
		return b + d;
	}
}*/


float BSpline::DeBoor(int p, int i, float t, vector<float> T)
{
    vector<vector<float>> P;
	P.push_back(vector<float>());
	for (int ii = 0; ii <= p; ++ii) {
		if (t >= T[i+ii] && t < T[i+ii+1])
			P[0].push_back(1);
		else
			P[0].push_back(0);
	}

	for (int pp = 1; pp <= p; ++pp) {
		P.push_back(vector<float>());
		for (int ii = 0; ii <= p - pp; ++ii)
		{
			float a, b, c, d, e;
			a = T[i+ii+pp] - T[i+ii];
			
			if (a == 0)
				b = 0;
			else
				b = P[pp-1][ii] * float(t - T[i+ii]) / a;

			c = T[i+ii+1+pp] - T[i+ii+1];
			if (c == 0)
				d = 0;
			else
				d = P[pp-1][ii+1] * float(T[i+ii+1+pp] - t) / c;
			
			e = b + d;
			
			P[pp].push_back(e);
		}
	}
	return P[p][0];
}


int BSpline::GetControlPointAtPosition(ofVec2f position)
{
	for (int i = 0; i < controlPoints.size(); ++i) {
		if (position.distance(controlPoints[i]) < 10)
			return i;
	}
	return -1;
}


void BSpline::UpdateDegree(int newDegree)
{
	degree = newDegree;
	CheckKnots();
}


void BSpline::CheckKnots()
{
	int delta = knots.size() - (controlPoints.size() + degree + 1);
	if (delta < 0) {
		for (int i = delta; i < 0; ++i) {
			if (knots.size() == 0)
				knots.push_back(0);
			else
				knots.push_back(knots[knots.size()-1] + 1);
		}
	}
	else if (delta > 0) {
		for (int i = 0; i < delta; ++i) {
			knots.pop_back();
		}
	}
}


void BSpline::MoveControl(int controlIndex, ofVec3f pos)
{
	controlPoints[controlIndex] = pos;
	UpdateLocalBSpline(controlIndex);
}


void BSpline::UpdateLocalBSpline(int controlIndex)
{
	for (int i = 0; i <= nbPas; ++i)
	{
		float t = float(i)/nbPas * (knots[knots.size()-degree-1] - knots[degree]) + knots[degree];
		if (t > knots[controlIndex] && t < knots[controlIndex + degree + 1]) {
			ofVec3f point = CalculateBSplinePoint(t, knots, controlPoints);
			bSplinePoints[i] = point;
		}
	}
}
