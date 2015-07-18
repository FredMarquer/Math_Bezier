#include "SceneBSpline.h"


SceneBSpline::SceneBSpline(void)
{
	isDragMode = false;
	isEditMode = false;
	nbPas = 50;
}


SceneBSpline::~SceneBSpline(void)
{
}

//--------------------------------------------------------------
void SceneBSpline::Init()
{
	newBSplineButton.addListener(this,&SceneBSpline::NewBSpline);
	editDegreeButton.addListener(this,&SceneBSpline::EditDegree);
	addControlButton.addListener(this,&SceneBSpline::AddControl);
	removeControlButton.addListener(this,&SceneBSpline::RemoveControl);
	editKnotsButton.addListener(this,&SceneBSpline::EditKnots);
	backButton.addListener(this,&SceneBSpline::Back);
	
	guiMain.setup();
	guiMain.setPosition(10, 100);
	guiMain.add(nbPasSlider.setup("nb Pas", 50, 10, 200));
	guiMain.add(newBSplineButton.setup("New BSpline"));
	
	guiEdit.setup();
	guiEdit.setPosition(10, 100);
	guiEdit.add(editDegreeButton.setup("Edit Degree"));
	guiEdit.add(addControlButton.setup("Add Control"));
	guiEdit.add(removeControlButton.setup("Remove Control"));
	guiEdit.add(editKnotsButton.setup("Edit Knots"));
	guiEdit.add(backButton.setup("Back"));
}

//--------------------------------------------------------------
void SceneBSpline::Update()
{
	if (nbPasSlider != nbPas)
		UpdateNbPas();

	if (isDragMode) {
		bSplines[draggedBSpline].MoveControl(draggedPoint, ofVec3f(ofGetMouseX(), ofGetMouseY(), 0));
		/*bSplines[draggedBSpline].controlPoints[draggedPoint] = ofVec3f(ofGetMouseX(), ofGetMouseY(), 0);
		bSplines[draggedBSpline].GenerateBSpline();*/
	}
}

//--------------------------------------------------------------
void SceneBSpline::Draw()
{
	for (int i = 0; i < bSplines.size(); ++i) {
		bSplines[i].Draw();
	}

	if (!isEditMode)
		guiMain.draw();
	else
		guiEdit.draw();
}

//--------------------------------------------------------------
void SceneBSpline::keyPressed(int key)
{
}

//--------------------------------------------------------------
void SceneBSpline::keyReleased(int key)
{
}

//--------------------------------------------------------------
void SceneBSpline::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void SceneBSpline::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void SceneBSpline::mousePressed(int x, int y, int button)
{
	if (button == 0) {
		ofVec2f mousePosition = ofVec2f(x, y);
		for (int i = 0; i < bSplines.size(); ++i) {
			int point = bSplines[i].GetControlPointAtPosition(mousePosition);
			if (point != -1) {
				isDragMode = true;
				draggedBSpline = i;
				draggedPoint = point;
				return;
			}
		}
	}
}

//--------------------------------------------------------------
void SceneBSpline::mouseReleased(int x, int y, int button)
{
	if (button == 0) {
		draggedBSpline = -1;
		draggedPoint = -1;
		isDragMode = false;
	}
}


void SceneBSpline::UpdateNbPas()
{
	nbPas = nbPasSlider;
	for (int i = 0; i < bSplines.size(); ++i) {
		bSplines[i].nbPas = nbPasSlider;
		bSplines[i].GenerateBSpline();
	}
}


void SceneBSpline::NewBSpline()
{
	bSplines.push_back(BSpline());
	selectedBSpline = bSplines.size() - 1;
	isEditMode = true;
}


void SceneBSpline::EditDegree()
{
	cout << "Degree ?" << endl;
	int degree = 0;
    cin >> degree;
	if (degree > 0 && degree < 10) {
		bSplines[selectedBSpline].UpdateDegree(degree);
		bSplines[selectedBSpline].GenerateBSpline();
	}
}

void SceneBSpline::AddControl()
{
	bSplines[selectedBSpline].controlPoints.push_back(ofVec3f(ofGetWindowWidth() * 0.5f, ofGetWindowHeight() * 0.5f, 0));
	bSplines[selectedBSpline].CheckKnots();
	bSplines[selectedBSpline].GenerateBSpline();
}


void SceneBSpline::RemoveControl()
{
	bSplines[selectedBSpline].controlPoints.pop_back();
	bSplines[selectedBSpline].CheckKnots();
	bSplines[selectedBSpline].GenerateBSpline();
}


void SceneBSpline::EditKnots()
{
	float min = 0;
	for (int i = 0; i < bSplines[selectedBSpline].knots.size(); ++i) {
		cout << "Knots " << i << " ?" << endl;
		float knots = 0;
		cin >> knots;
		if (knots < min)
			knots = min;
		bSplines[selectedBSpline].knots[i] = knots;
		min = knots;
	}
	bSplines[selectedBSpline].GenerateBSpline();
}


void SceneBSpline::Back()
{
	selectedBSpline = -1;
	isEditMode = false;
}
