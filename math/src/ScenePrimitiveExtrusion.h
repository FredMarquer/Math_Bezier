#pragma once

#include "SceneBase.h"
#include "PrimitiveExtrusion.h"

class ScenePrimitiveExtrusion: public SceneBase
{
public:
	ScenePrimitiveExtrusion(void);
	~ScenePrimitiveExtrusion(void);

	void Init() override;
	void Update() override;
	void Draw() override;
	void keyPressed(int key) override;
	void keyReleased(int key) override;
	void mouseMoved(int x, int y) override;
	void mouseDragged(int x, int y, int button) override;
	void mousePressed(int x, int y, int button) override;
	void mouseReleased(int x, int y, int button) override;

	ofEasyCam cam;

	ofxPanel gui;

	PrimitiveExtrusion primitiveExtrusion;
};

