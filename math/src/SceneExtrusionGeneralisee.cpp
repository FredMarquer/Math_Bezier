#include "SceneExtrusionGeneralisee.h"


SceneExtrusionGeneralisee::SceneExtrusionGeneralisee(void)
{
}


SceneExtrusionGeneralisee::~SceneExtrusionGeneralisee(void)
{
}


//--------------------------------------------------------------
void SceneExtrusionGeneralisee::Init()
{
	gui.setup();
	gui.setPosition(10, 100);
}

//--------------------------------------------------------------
void SceneExtrusionGeneralisee::Update()
{
}

//--------------------------------------------------------------
void SceneExtrusionGeneralisee::Draw()
{
	ofEnableDepthTest();
	cam.begin();
	extrusionGeneralisee.Draw();
	cam.end();
	ofDisableDepthTest();
	gui.draw();
}

//--------------------------------------------------------------
void SceneExtrusionGeneralisee::keyPressed(int key)
{
}

//--------------------------------------------------------------
void SceneExtrusionGeneralisee::keyReleased(int key)
{
}

//--------------------------------------------------------------
void SceneExtrusionGeneralisee::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void SceneExtrusionGeneralisee::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void SceneExtrusionGeneralisee::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void SceneExtrusionGeneralisee::mouseReleased(int x, int y, int button)
{
}
