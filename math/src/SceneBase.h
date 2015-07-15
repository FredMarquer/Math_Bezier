#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class SceneBase
{
public:
	SceneBase(void);
	~SceneBase(void);

	virtual void Init(){};
	virtual void Update(){};
	virtual void Draw(){};
	virtual void keyPressed(int key){};
	virtual void keyReleased(int key){};
	virtual void mouseMoved(int x, int y){};
	virtual void mouseDragged(int x, int y, int button){};
	virtual void mousePressed(int x, int y, int button){};
	virtual void mouseReleased(int x, int y, int button){};
};

