#include "SceneExtrusionRevolution.h"


SceneExtrusionRevolution::SceneExtrusionRevolution(void)
{
	isBSplineSetted = false;

	isDragMode = false;
	draggedPoint = -1;

	selectedControl = 0;

	nbPas = 20;
	angle = 360;
}


SceneExtrusionRevolution::~SceneExtrusionRevolution(void)
{
}


//--------------------------------------------------------------
void SceneExtrusionRevolution::Init()
{
	editDegreeButton.addListener(this,&SceneExtrusionRevolution::EditDegree);
	addControlButton.addListener(this,&SceneExtrusionRevolution::AddControl);
	removeControlButton.addListener(this,&SceneExtrusionRevolution::RemoveControl);
	editKnotsButton.addListener(this,&SceneExtrusionRevolution::EditKnots);
	okButton.addListener(this,&SceneExtrusionRevolution::Ok);

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
	guiExtrusion.add(angleSlider.setup("Angle", 360, 1, 360));
}

//--------------------------------------------------------------
void SceneExtrusionRevolution::Update()
{
	if (!isBSplineSetted)
	{
		if (isDragMode)
			extrusionRevolution.bSplineBase.MoveControl(draggedPoint, ofVec3f(ofGetMouseX(), ofGetMouseY(), 0));
	}
	else
	{
		if (nbPasSlider != nbPas) {
			nbPas = nbPasSlider;
			extrusionRevolution.bSplineBase.nbPas = nbPas;
			extrusionRevolution.bSplineBase.GenerateBSpline();
			extrusionRevolution.nbPas = nbPasSlider;
			extrusionRevolution.Extrude();
		}

		if (angleSlider != angle) {
			angle = angleSlider;
			extrusionRevolution.angle = angleSlider;
			extrusionRevolution.Extrude();
		}
	}
}

//--------------------------------------------------------------
void SceneExtrusionRevolution::Draw()
{
	if (!isBSplineSetted)
	{
		extrusionRevolution.bSplineBase.Draw();
		extrusionRevolution.bSplineBase.DrawPoints();

		guiBSpline.draw();
	}
	else
	{
		ofEnableDepthTest();
		cam.begin();

		extrusionRevolution.Draw();

		ofPushMatrix();
		ofTranslate(extrusionRevolution.bSplineBase.controlPoints[selectedControl]);
		ofFill();
		ofSetColor(ofColor::white);
		ofDrawBox(10);
		ofNoFill();
		ofSetColor(ofColor::black);
		ofDrawBox(10);
		ofPopMatrix();

		cam.end();
		ofDisableDepthTest();

		guiExtrusion.draw();
	}
}

//--------------------------------------------------------------
void SceneExtrusionRevolution::keyPressed(int key)
{
	if (isBSplineSetted) {
		switch (key) {
		case 356: //left
			--selectedControl;
			if(selectedControl < 0)
				selectedControl = 0;
			break;
		case 358: //rigth
			++selectedControl;
			if(selectedControl >= extrusionRevolution.bSplineBase.controlPoints.size())
				selectedControl = extrusionRevolution.bSplineBase.controlPoints.size() - 1;
			break;
		case 50: //2
			extrusionRevolution.bSplineBase.controlPoints[selectedControl] += ofVec3f(0, -10, 0);
			extrusionRevolution.bSplineBase.GenerateBSpline();
			extrusionRevolution.Extrude();
			break;
		case 52: //4
			extrusionRevolution.bSplineBase.controlPoints[selectedControl] += ofVec3f(-10, 0, 0);
			extrusionRevolution.bSplineBase.GenerateBSpline();
			extrusionRevolution.Extrude();
			break;
		case 54: //6
			extrusionRevolution.bSplineBase.controlPoints[selectedControl] += ofVec3f(10, 0, 0);
			extrusionRevolution.bSplineBase.GenerateBSpline();
			extrusionRevolution.Extrude();
			break;
		case 56: //8
			extrusionRevolution.bSplineBase.controlPoints[selectedControl] += ofVec3f(0, 10, 0);
			extrusionRevolution.bSplineBase.GenerateBSpline();
			extrusionRevolution.Extrude();
			break;
		}
	}
}

//--------------------------------------------------------------
void SceneExtrusionRevolution::keyReleased(int key)
{
}

//--------------------------------------------------------------
void SceneExtrusionRevolution::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void SceneExtrusionRevolution::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void SceneExtrusionRevolution::mousePressed(int x, int y, int button)
{
	if (!isBSplineSetted)
	{
		if (button == 0) {
			ofVec2f mousePosition = ofVec2f(x, y);
			int point = extrusionRevolution.bSplineBase.GetControlPointAtPosition(mousePosition);
			if (point != -1) {
				isDragMode = true;
				draggedPoint = point;
				return;
			}
		}
	}
}

//--------------------------------------------------------------
void SceneExtrusionRevolution::mouseReleased(int x, int y, int button)
{
	if (button == 0) {
		draggedPoint = -1;
		isDragMode = false;
	}
}


void SceneExtrusionRevolution::EditDegree()
{
	cout << "Degree ?" << endl;
	int degree = 0;
    cin >> degree;
	if (degree > 0 && degree < 10) {
		extrusionRevolution.bSplineBase.UpdateDegree(degree);
		extrusionRevolution.bSplineBase.GenerateBSpline();
	}
}

void SceneExtrusionRevolution::AddControl()
{
	extrusionRevolution.bSplineBase.controlPoints.push_back(ofVec3f(ofGetWindowWidth() * 0.5f, ofGetWindowHeight() * 0.5f, 0));
	extrusionRevolution.bSplineBase.CheckKnots();
	extrusionRevolution.bSplineBase.GenerateBSpline();
}


void SceneExtrusionRevolution::RemoveControl()
{
	extrusionRevolution.bSplineBase.controlPoints.pop_back();
	extrusionRevolution.bSplineBase.CheckKnots();
	extrusionRevolution.bSplineBase.GenerateBSpline();
}


void SceneExtrusionRevolution::EditKnots()
{
	float min = 0;
	for (int i = 0; i < extrusionRevolution.bSplineBase.knots.size(); ++i) {
		cout << "Knots " << i << " ?" << endl;
		float knots = 0;
		cin >> knots;
		if (knots < min)
			knots = min;
		extrusionRevolution.bSplineBase.knots[i] = knots;
		min = knots;
	}
	extrusionRevolution.bSplineBase.GenerateBSpline();
}


void SceneExtrusionRevolution::Ok()
{
	if (extrusionRevolution.bSplineBase.bSplinePoints.size() > 0) {
		isBSplineSetted = true;
		float offset = 0;
		for (int i = 0; i < extrusionRevolution.bSplineBase.controlPoints.size(); ++i)
			offset += extrusionRevolution.bSplineBase.controlPoints[i].y;
		offset /= extrusionRevolution.bSplineBase.controlPoints.size();
		for (int i = 0; i < extrusionRevolution.bSplineBase.controlPoints.size(); ++i) {
			extrusionRevolution.bSplineBase.controlPoints[i].y -= offset;
		}
		extrusionRevolution.bSplineBase.GenerateBSpline();
		extrusionRevolution.Extrude();
	}
}
