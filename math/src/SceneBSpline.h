#pragma once

#include "SceneBase.h"
#include "BSpline.h"

class SceneBSpline: public SceneBase
{
public:
	SceneBSpline(void);
	~SceneBSpline(void);

	void Init() override;
	void Update() override;
	void Draw() override;
	void keyPressed(int key) override;
	void keyReleased(int key) override;
	void mouseMoved(int x, int y) override;
	void mouseDragged(int x, int y, int button) override;
	void mousePressed(int x, int y, int button) override;
	void mouseReleased(int x, int y, int button) override;

	void UpdateNbPas();

	void NewBSpline();
	void EditDegree();
	void AddControl();
	void RemoveControl();
	void EditKnots();
	void Back();

	ofxPanel guiMain;
	ofxIntSlider nbPasSlider;
	ofxButton newBSplineButton;

	ofxPanel guiEdit;
	ofxButton editDegreeButton;
	ofxButton addControlButton;
	ofxButton removeControlButton;
	ofxButton editKnotsButton;
	ofxButton backButton;

	float nbPas;

	vector<BSpline> bSplines;

	bool isDragMode;
	int draggedBSpline;
	int draggedPoint;

	bool isEditMode;
	int selectedBSpline;
};

