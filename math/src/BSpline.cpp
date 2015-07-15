#include "BSpline.h"


BSpline::BSpline(void)
{
	nbPas = 50;
	controlPoints.push_back(ofVec3f(200, 600, 0));
	controlPoints.push_back(ofVec3f(250, 200, 0));
	controlPoints.push_back(ofVec3f(300, 600, 0));
	controlPoints.push_back(ofVec3f(350, 200, 0));
	controlPoints.push_back(ofVec3f(400, 600, 0));
	controlPoints.push_back(ofVec3f(450, 200, 0));
	controlPoints.push_back(ofVec3f(500, 600, 0));
	controlPoints.push_back(ofVec3f(550, 200, 0));
	controlPoints.push_back(ofVec3f(600, 600, 0));
	knots.push_back(1.0f);
	knots.push_back(2.0f);
	knots.push_back(3.0f);
	knots.push_back(4.0f);
	knots.push_back(5.0f);
	knots.push_back(6.0f);
	knots.push_back(7.0f);
	knots.push_back(8.0f);
	knots.push_back(9.0f);
	knots.push_back(10.0f);
	knots.push_back(11.0f);
	knots.push_back(12.0f);
	knots.push_back(13.0f);
	GenerateBSpline();
}


BSpline::~BSpline(void)
{
}


void BSpline::Draw()
{
	ofSetColor(ofColor::red);
	int imax  = bSplinePoints.size() - 1;
	for (int i = 0; i < imax; ++i)
		ofLine(bSplinePoints[i], bSplinePoints[i+1]);

	ofSetColor(ofColor::black);
	imax = controlPoints.size() - 1;
	for (int i = 0; i < imax; ++i) 
		ofLine(controlPoints[i], controlPoints[i+1]);
}


void BSpline::GenerateBSpline()
{
	bSplinePoints.clear();
	for (int i = 0; i <= nbPas; ++i) {
		ofVec3f point = CalculateBSplinePoint(float(i)/nbPas, knots, controlPoints);
		bSplinePoints.push_back(point);
	}
}


ofVec3f BSpline::CalculateBSplinePoint(float t, vector<float> T, vector<ofVec3f> controls)
{
	int p = 3;
	t = t * (knots[knots.size()-p-1] - knots[p]) + knots[p];

	ofVec3f point = ofVec3f(0, 0, 0);
	for (int i = 0; i < controls.size(); ++i)
		point += controls[i] * DeBoor(p, i, t, T);

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
}
*/

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
