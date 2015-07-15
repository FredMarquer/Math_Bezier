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

	ofxPanel gui;
	ofxIntSlider nbPasSlider;

	float nbPas;

	BSpline bSpline;
};

