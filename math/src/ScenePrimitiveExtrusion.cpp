#include "ScenePrimitiveExtrusion.h"


ScenePrimitiveExtrusion::ScenePrimitiveExtrusion(void)
{
	primitiveExtrusion = PrimitiveExtrusion();
}


ScenePrimitiveExtrusion::~ScenePrimitiveExtrusion(void)
{
}

//--------------------------------------------------------------
void ScenePrimitiveExtrusion::Init()
{
	gui.setup();
	gui.setPosition(10, 100);
}

//--------------------------------------------------------------
void ScenePrimitiveExtrusion::Update()
{
}

//--------------------------------------------------------------
void ScenePrimitiveExtrusion::Draw()
{
	ofEnableDepthTest();
	cam.begin();
	primitiveExtrusion.Draw();
	cam.end();
	ofDisableDepthTest();
	gui.draw();
}

//--------------------------------------------------------------
void ScenePrimitiveExtrusion::keyPressed(int key)
{
}

//--------------------------------------------------------------
void ScenePrimitiveExtrusion::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ScenePrimitiveExtrusion::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ScenePrimitiveExtrusion::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ScenePrimitiveExtrusion::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ScenePrimitiveExtrusion::mouseReleased(int x, int y, int button)
{
}
