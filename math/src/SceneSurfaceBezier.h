#pragma once

#include "SceneBase.h"
#include "SurfaceBezier.h"

class SceneSurfaceBezier: public SceneBase
{
public:
	SceneSurfaceBezier(void);
	~SceneSurfaceBezier(void);

	void Init() override;
	void Update() override;
	void Draw() override;
	void keyPressed(int key) override;
	void keyReleased(int key) override;
	void mouseMoved(int x, int y) override;
	void mouseDragged(int x, int y, int button) override;
	void mousePressed(int x, int y, int button) override;
	void mouseReleased(int x, int y, int button) override;

	void StartButton();

	void UpdateNbPas();

	ofEasyCam cam;

	bool isPlaying;

	ofxPanel startGui;
	ofxIntSlider nbControlsSlider;
	ofxButton startButton;

	ofxPanel mainGui;
	ofxIntSlider nbPasSlider;

	float nbPas;
	int selectedControlX;
	int selectedControlY;

	SurfaceBezier surfaceBezier;
};

