#include "SceneExtrusionGeneralisee.h"


SceneExtrusionGeneralisee::SceneExtrusionGeneralisee(void)
{
	isBaseSetted = false;
	isGuideSetted = false;

	isDragMode = false;
	draggedPoint = -1;

	nbPas = 20;
}


SceneExtrusionGeneralisee::~SceneExtrusionGeneralisee(void)
{
}


//--------------------------------------------------------------
void SceneExtrusionGeneralisee::Init()
{
	editDegreeButton.addListener(this,&SceneExtrusionGeneralisee::EditDegree);
	addControlButton.addListener(this,&SceneExtrusionGeneralisee::AddControl);
	removeControlButton.addListener(this,&SceneExtrusionGeneralisee::RemoveControl);
	editKnotsButton.addListener(this,&SceneExtrusionGeneralisee::EditKnots);
	okButton.addListener(this,&SceneExtrusionGeneralisee::Ok);

	guiBSpline.setup();
	guiBSpline.setPosition(10, 100);
	guiBSpline.add(editDegreeButton.setup("Edit Degree"));
	guiBSpline.add(addControlButton.setup("Add Control"));
	guiBSpline.add(removeControlButton.setup("Remove Control"));
	guiBSpline.add(editKnotsButton.setup("Edit Knots"));
	guiBSpline.add(okButton.setup("Ok"));

	guiExtrusion.setup();
	guiExtrusion.setPosition(10, 100);
	guiExtrusion.add(nbPasSlider.setup("nb Pas", 20, 5, 100));
}

//--------------------------------------------------------------
void SceneExtrusionGeneralisee::Update()
{
	if (!isBaseSetted)
	{
		if (isDragMode)
			extrusionGeneralisee.bSplineBase.MoveControl(draggedPoint, ofVec3f(ofGetMouseX(), ofGetMouseY(), 0));
	}
	else if (!isGuideSetted)
	{
		if (isDragMode)
			extrusionGeneralisee.bSplineGuide.MoveControl(draggedPoint, ofVec3f(ofGetMouseX(), ofGetMouseY(), 0));
	}
	else
	{
		if (nbPasSlider != nbPas) {
			nbPas = nbPasSlider;
			extrusionGeneralisee.bSplineBase.nbPas = nbPas;
			extrusionGeneralisee.bSplineBase.GenerateBSpline();
			extrusionGeneralisee.bSplineGuide.nbPas = nbPas;
			extrusionGeneralisee.bSplineGuide.GenerateBSpline();
			extrusionGeneralisee.Extrude();
		}
	}
}

//--------------------------------------------------------------
void SceneExtrusionGeneralisee::Draw()
{
	if (!isBaseSetted)
	{
		extrusionGeneralisee.bSplineBase.Draw();
		extrusionGeneralisee.bSplineBase.DrawPoints();
		guiBSpline.draw();
	}
	else if (!isGuideSetted)
	{
		extrusionGeneralisee.bSplineGuide.Draw();
		extrusionGeneralisee.bSplineGuide.DrawPoints();
		guiBSpline.draw();
	}
	else
	{
		ofEnableDepthTest();
		cam.begin();

		extrusionGeneralisee.Draw();

		cam.end();
		ofDisableDepthTest();

		guiExtrusion.draw();
	}
}

//--------------------------------------------------------------
void SceneExtrusionGeneralisee::keyPressed(int key)
{
}

//--------------------------------------------------------------
void SceneExtrusionGeneralisee::keyReleased(int key)
{
}

//--------------------------------------------------------------
void SceneExtrusionGeneralisee::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void SceneExtrusionGeneralisee::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void SceneExtrusionGeneralisee::mousePressed(int x, int y, int button)
{
	if (!isBaseSetted)
	{
		if (button == 0) {
			ofVec2f mousePosition = ofVec2f(x, y);
			int point = extrusionGeneralisee.bSplineBase.GetControlPointAtPosition(mousePosition);
			if (point != -1) {
				isDragMode = true;
				draggedPoint = point;
				return;
			}
		}
	}
	else if (!isGuideSetted)
	{
		if (button == 0) {
			ofVec2f mousePosition = ofVec2f(x, y);
			int point = extrusionGeneralisee.bSplineGuide.GetControlPointAtPosition(mousePosition);
			if (point != -1) {
				isDragMode = true;
				draggedPoint = point;
				return;
			}
		}
	}
}

//--------------------------------------------------------------
void SceneExtrusionGeneralisee::mouseReleased(int x, int y, int button)
{
	if (button == 0) {
		draggedPoint = -1;
		isDragMode = false;
	}
}


void SceneExtrusionGeneralisee::EditDegree()
{
	cout << "Degree ?" << endl;
	int degree = 0;
    cin >> degree;
	if (degree > 0 && degree < 10) {
		if (!isBaseSetted)
		{
			extrusionGeneralisee.bSplineBase.UpdateDegree(degree);
			extrusionGeneralisee.bSplineBase.GenerateBSpline();
		}
		else if (!isGuideSetted)
		{
			extrusionGeneralisee.bSplineGuide.UpdateDegree(degree);
			extrusionGeneralisee.bSplineGuide.GenerateBSpline();
		}
	}
}

void SceneExtrusionGeneralisee::AddControl()
{
	if (!isBaseSetted)
	{
		extrusionGeneralisee.bSplineBase.controlPoints.push_back(ofVec3f(ofGetWindowWidth() * 0.5f, ofGetWindowHeight() * 0.5f, 0));
		extrusionGeneralisee.bSplineBase.CheckKnots();
		extrusionGeneralisee.bSplineBase.GenerateBSpline();
	}
	else if (!isGuideSetted)
	{
		extrusionGeneralisee.bSplineGuide.controlPoints.push_back(ofVec3f(ofGetWindowWidth() * 0.5f, ofGetWindowHeight() * 0.5f, 0));
		extrusionGeneralisee.bSplineGuide.CheckKnots();
		extrusionGeneralisee.bSplineGuide.GenerateBSpline();
	}
}


void SceneExtrusionGeneralisee::RemoveControl()
{
	if (!isBaseSetted)
	{
		extrusionGeneralisee.bSplineBase.controlPoints.pop_back();
		extrusionGeneralisee.bSplineBase.CheckKnots();
		extrusionGeneralisee.bSplineBase.GenerateBSpline();
	}
	else if (!isGuideSetted)
	{
		extrusionGeneralisee.bSplineGuide.controlPoints.pop_back();
		extrusionGeneralisee.bSplineGuide.CheckKnots();
		extrusionGeneralisee.bSplineGuide.GenerateBSpline();
	}
}


void SceneExtrusionGeneralisee::EditKnots()
{
	if (!isBaseSetted)
	{
		float min = 0;
		for (int i = 0; i < extrusionGeneralisee.bSplineBase.knots.size(); ++i) {
			cout << "Knots " << i << " ?" << endl;
			float knots = 0;
			cin >> knots;
			if (knots < min)
				knots = min;
			extrusionGeneralisee.bSplineBase.knots[i] = knots;
			min = knots;
		}
		extrusionGeneralisee.bSplineBase.GenerateBSpline();
	}
	else if (!isGuideSetted)
	{
		float min = 0;
		for (int i = 0; i < extrusionGeneralisee.bSplineGuide.knots.size(); ++i) {
			cout << "Knots " << i << " ?" << endl;
			float knots = 0;
			cin >> knots;
			if (knots < min)
				knots = min;
			extrusionGeneralisee.bSplineGuide.knots[i] = knots;
			min = knots;
		}
		extrusionGeneralisee.bSplineGuide.GenerateBSpline();
	}
}


void SceneExtrusionGeneralisee::Ok()
{
	if (!isBaseSetted)
	{
		if (extrusionGeneralisee.bSplineBase.bSplinePoints.size() > 0) {
			isBaseSetted = true;
			ofVec3f offset;
			for (int i = 0; i < extrusionGeneralisee.bSplineBase.controlPoints.size(); ++i)
				offset += extrusionGeneralisee.bSplineBase.controlPoints[i];
			offset /= extrusionGeneralisee.bSplineBase.controlPoints.size();
			for (int i = 0; i < extrusionGeneralisee.bSplineBase.controlPoints.size(); ++i) {
				ofVec3f point = extrusionGeneralisee.bSplineBase.controlPoints[i] - offset;
				extrusionGeneralisee.bSplineBase.controlPoints[i] = ofVec3f(point.x, point.z, point.y);
			}
			extrusionGeneralisee.bSplineBase.GenerateBSpline();
		}
	}
	else if (!isGuideSetted)
	{
		if (extrusionGeneralisee.bSplineGuide.bSplinePoints.size() > 0) {
			isGuideSetted = true;
			extrusionGeneralisee.Extrude();
		}
	}
}
