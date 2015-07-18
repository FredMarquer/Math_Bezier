#include "SceneExtrusionSimple.h"


SceneExtrusionSimple::SceneExtrusionSimple(void)
{
}


SceneExtrusionSimple::~SceneExtrusionSimple(void)
{
}


//--------------------------------------------------------------
void SceneExtrusionSimple::Init()
{
	gui.setup();
	gui.setPosition(10, 100);
}

//--------------------------------------------------------------
void SceneExtrusionSimple::Update()
{
}

//--------------------------------------------------------------
void SceneExtrusionSimple::Draw()
{
	ofEnableDepthTest();
	cam.begin();
	extrusionSimple.Draw();
	cam.end();
	ofDisableDepthTest();
	gui.draw();
}

//--------------------------------------------------------------
void SceneExtrusionSimple::keyPressed(int key)
{
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
}

//--------------------------------------------------------------
void SceneExtrusionSimple::mouseReleased(int x, int y, int button)
{
}
