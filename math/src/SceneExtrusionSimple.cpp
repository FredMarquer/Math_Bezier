#include "SceneExtrusionSimple.h"


SceneExtrusionSimple::SceneExtrusionSimple(void)
{
	isBSplineSetted = false;

	isDragMode = false;
	draggedPoint = -1;

	selectedControl = 0;

	nbPas = 20;
	height = 100;
	sizeRatio = 1;
}


SceneExtrusionSimple::~SceneExtrusionSimple(void)
{
}


//--------------------------------------------------------------
void SceneExtrusionSimple::Init()
{
	editDegreeButton.addListener(this,&SceneExtrusionSimple::EditDegree);
	addControlButton.addListener(this,&SceneExtrusionSimple::AddControl);
	removeControlButton.addListener(this,&SceneExtrusionSimple::RemoveControl);
	editKnotsButton.addListener(this,&SceneExtrusionSimple::EditKnots);
	okButton.addListener(this,&SceneExtrusionSimple::Ok);

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
	guiExtrusion.add(heightSlider.setup("Hauteur", 100, 1, 500));
	guiExtrusion.add(sizeRatioSlider.setup("Scale partie haute", 1, 0, 5));
}

//--------------------------------------------------------------
void SceneExtrusionSimple::Update()
{
	if (!isBSplineSetted)
	{
		if (isDragMode)
			extrusionSimple.bSplineBase.MoveControl(draggedPoint, ofVec3f(ofGetMouseX(), ofGetMouseY(), 0));
	}
	else
	{
		if (nbPasSlider != nbPas) {
			nbPas = nbPasSlider;
			extrusionSimple.bSplineBase.nbPas = nbPas;
			extrusionSimple.bSplineBase.GenerateBSpline();
			extrusionSimple.nbPas = nbPasSlider;
			extrusionSimple.Extrude();
		}

		if (heightSlider != height) {
			height = heightSlider;
			extrusionSimple.height = heightSlider;
			extrusionSimple.Extrude();
		}

		if (sizeRatioSlider != sizeRatio) {
			sizeRatio = sizeRatioSlider;
			extrusionSimple.sizeRatio = sizeRatioSlider;
			extrusionSimple.Extrude();
		}
	}
}

//--------------------------------------------------------------
void SceneExtrusionSimple::Draw()
{
	if (!isBSplineSetted)
	{
		extrusionSimple.bSplineBase.Draw();
		extrusionSimple.bSplineBase.DrawPoints();
		guiBSpline.draw();
	}
	else
	{
		ofEnableDepthTest();
		cam.begin();

		extrusionSimple.Draw();

		ofPushMatrix();
		ofTranslate(extrusionSimple.bSplineBase.controlPoints[selectedControl]);
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
void SceneExtrusionSimple::keyPressed(int key)
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
			if(selectedControl >= extrusionSimple.bSplineBase.controlPoints.size())
				selectedControl = extrusionSimple.bSplineBase.controlPoints.size() - 1;
			break;
		case 50: //2
			extrusionSimple.bSplineBase.controlPoints[selectedControl] += ofVec3f(0, 0, 10);
			extrusionSimple.bSplineBase.GenerateBSpline();
			extrusionSimple.Extrude();
			break;
		case 52: //4
			extrusionSimple.bSplineBase.controlPoints[selectedControl] += ofVec3f(-10, 0, 0);
			extrusionSimple.bSplineBase.GenerateBSpline();
			extrusionSimple.Extrude();
			break;
		case 54: //6
			extrusionSimple.bSplineBase.controlPoints[selectedControl] += ofVec3f(10, 0, 0);
			extrusionSimple.bSplineBase.GenerateBSpline();
			extrusionSimple.Extrude();
			break;
		case 56: //8
			extrusionSimple.bSplineBase.controlPoints[selectedControl] += ofVec3f(0, 0, -10);
			extrusionSimple.bSplineBase.GenerateBSpline();
			extrusionSimple.Extrude();
			break;
		}
	}
}

//--------------------------------------------------------------
void SceneExtrusionSimple::keyReleased(int key)
{
}

//--------------------------------------------------------------
void SceneExtrusionSimple::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void SceneExtrusionSimple::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void SceneExtrusionSimple::mousePressed(int x, int y, int button)
{
	if (!isBSplineSetted)
	{
		if (button == 0) {
			ofVec2f mousePosition = ofVec2f(x, y);
			int point = extrusionSimple.bSplineBase.GetControlPointAtPosition(mousePosition);
			if (point != -1) {
				isDragMode = true;
				draggedPoint = point;
				return;
			}
		}
	}
}

//--------------------------------------------------------------
void SceneExtrusionSimple::mouseReleased(int x, int y, int button)
{
	if (button == 0) {
		draggedPoint = -1;
		isDragMode = false;
	}
}


void SceneExtrusionSimple::EditDegree()
{
	cout << "Degree ?" << endl;
	int degree = 0;
    cin >> degree;
	if (degree > 0 && degree < 10) {
		extrusionSimple.bSplineBase.UpdateDegree(degree);
		extrusionSimple.bSplineBase.GenerateBSpline();
	}
}

void SceneExtrusionSimple::AddControl()
{
	extrusionSimple.bSplineBase.controlPoints.push_back(ofVec3f(ofGetWindowWidth() * 0.5f, ofGetWindowHeight() * 0.5f, 0));
	extrusionSimple.bSplineBase.CheckKnots();
	extrusionSimple.bSplineBase.GenerateBSpline();
}


void SceneExtrusionSimple::RemoveControl()
{
	extrusionSimple.bSplineBase.controlPoints.pop_back();
	extrusionSimple.bSplineBase.CheckKnots();
	extrusionSimple.bSplineBase.GenerateBSpline();
}


void SceneExtrusionSimple::EditKnots()
{
	float min = 0;
	for (int i = 0; i < extrusionSimple.bSplineBase.knots.size(); ++i) {
		cout << "Knots " << i << " ?" << endl;
		float knots = 0;
		cin >> knots;
		if (knots < min)
			knots = min;
		extrusionSimple.bSplineBase.knots[i] = knots;
		min = knots;
	}
	extrusionSimple.bSplineBase.GenerateBSpline();
}


void SceneExtrusionSimple::Ok()
{
	if (extrusionSimple.bSplineBase.bSplinePoints.size() > 0) {
		isBSplineSetted = true;
		ofVec3f offset;
		for (int i = 0; i < extrusionSimple.bSplineBase.controlPoints.size(); ++i)
			offset += extrusionSimple.bSplineBase.controlPoints[i];
		offset /= extrusionSimple.bSplineBase.controlPoints.size();
		for (int i = 0; i < extrusionSimple.bSplineBase.controlPoints.size(); ++i) {
			ofVec3f point = extrusionSimple.bSplineBase.controlPoints[i] - offset;
			extrusionSimple.bSplineBase.controlPoints[i] = ofVec3f(point.x, point.z, point.y);
		}
		extrusionSimple.bSplineBase.GenerateBSpline();
		extrusionSimple.Extrude();
	}
}
