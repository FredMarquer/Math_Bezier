#include "SceneExtrusionRevolution.h"


SceneExtrusionRevolution::SceneExtrusionRevolution(void)
{
}


SceneExtrusionRevolution::~SceneExtrusionRevolution(void)
{
}


//--------------------------------------------------------------
void SceneExtrusionRevolution::Init()
{
	gui.setup();
	gui.setPosition(10, 100);
}

//--------------------------------------------------------------
void SceneExtrusionRevolution::Update()
{
}

//--------------------------------------------------------------
void SceneExtrusionRevolution::Draw()
{
	ofEnableDepthTest();
	cam.begin();
	extrusionRevolution.Draw();
	cam.end();
	ofDisableDepthTest();
	gui.draw();
}

//--------------------------------------------------------------
void SceneExtrusionRevolution::keyPressed(int key)
{
}

//--------------------------------------------------------------
void SceneExtrusionRevolution::keyReleased(int key)
{
}

//--------------------------------------------------------------
void SceneExtrusionRevolution::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void SceneExtrusionRevolution::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void SceneExtrusionRevolution::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void SceneExtrusionRevolution::mouseReleased(int x, int y, int button)
{
}
