#pragma once

#include "SceneBase.h"
#include "ExtrusionGeneralisee.h"

class SceneExtrusionGeneralisee: public SceneBase
{
public:
	SceneExtrusionGeneralisee(void);
	~SceneExtrusionGeneralisee(void);

	void Init() override;
	void Update() override;
	void Draw() override;
	void keyPressed(int key) override;
	void keyReleased(int key) override;
	void mouseMoved(int x, int y) override;
	void mouseDragged(int x, int y, int button) override;
	void mousePressed(int x, int y, int button) override;
	void mouseReleased(int x, int y, int button) override;

	void EditDegree();
	void AddControl();
	void RemoveControl();
	void EditKnots();
	void Ok();

	ofEasyCam cam;

	bool isBaseSetted;
	bool isGuideSetted;

	ofxPanel guiBSpline;
	ofxButton editDegreeButton;
	ofxButton addControlButton;
	ofxButton removeControlButton;
	ofxButton editKnotsButton;
	ofxButton okButton;

	ofxPanel guiExtrusion;
	ofxIntSlider nbPasSlider;
	ofxFloatSlider heightSlider;
	ofxFloatSlider sizeRatioSlider;

	bool isDragMode;
	int draggedPoint;

	int nbPas;

	ExtrusionGeneralisee extrusionGeneralisee;
};

