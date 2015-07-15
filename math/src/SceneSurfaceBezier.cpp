#include "SceneSurfaceBezier.h"


SceneSurfaceBezier::SceneSurfaceBezier(void)
{
	isPlaying = false;
	nbPas = 20;
	selectedControlX = 0;
	selectedControlY = 0;
}


SceneSurfaceBezier::~SceneSurfaceBezier(void)
{
}

//--------------------------------------------------------------
void SceneSurfaceBezier::Init()
{
	startButton.addListener(this,&SceneSurfaceBezier::StartButton);

	startGui.setup();
	startGui.setPosition(10, 100);
	startGui.add(nbControlsSlider.setup("nb Controls", 4, 3, 10));
	startGui.add(startButton.setup("Start"));

	mainGui.setup();
	mainGui.setPosition(10, 100);
	mainGui.add(nbPasSlider.setup("nb Pas", 20, 5, 100));
}

//--------------------------------------------------------------
void SceneSurfaceBezier::Update()
{
	if (nbPasSlider != nbPas)
		UpdateNbPas();

}

//--------------------------------------------------------------
void SceneSurfaceBezier::Draw()
{
	if (!isPlaying)
		startGui.draw();
	else {
		ofEnableDepthTest();
		cam.begin();

		surfaceBezier.Draw();

		ofPushMatrix();
		ofTranslate(surfaceBezier.controlPoints[selectedControlX][selectedControlY]);
		ofFill();
		ofSetColor(ofColor::white);
		ofDrawBox(10);
		ofNoFill();
		ofSetColor(ofColor::black);
		ofDrawBox(10);
		ofPopMatrix();

		cam.end();
		ofDisableDepthTest();

		mainGui.draw();
	}
}

//--------------------------------------------------------------
void SceneSurfaceBezier::keyPressed(int key)
{
	switch (key) {
	case 356: //left
		--selectedControlX;
		if(selectedControlX < 0)
			selectedControlX = 0;
		break;
	case 357: //up
		++selectedControlY;
		if(selectedControlY >= nbControlsSlider)
			selectedControlY = nbControlsSlider - 1;
		break;
	case 358: //rigth
		++selectedControlX;
		if(selectedControlX >= nbControlsSlider)
			selectedControlX = nbControlsSlider - 1;
		break;
	case 359: //down
		--selectedControlY;
		if(selectedControlY < 0)
			selectedControlY = 0;
		break;
	case 50: //2
		surfaceBezier.controlPoints[selectedControlX][selectedControlY] += ofVec3f(0, 0, 10);
		surfaceBezier.GenerateBezierSurface();
		break;
	case 51: //3
		surfaceBezier.controlPoints[selectedControlX][selectedControlY] += ofVec3f(0, -10, 0);
		surfaceBezier.GenerateBezierSurface();
		break;
	case 52: //4
		surfaceBezier.controlPoints[selectedControlX][selectedControlY] += ofVec3f(-10, 0, 0);
		surfaceBezier.GenerateBezierSurface();
		break;
	case 54: //6
		surfaceBezier.controlPoints[selectedControlX][selectedControlY] += ofVec3f(10, 0, 0);
		surfaceBezier.GenerateBezierSurface();
		break;
	case 56: //8
		surfaceBezier.controlPoints[selectedControlX][selectedControlY] += ofVec3f(0, 0, -10);
		surfaceBezier.GenerateBezierSurface();
		break;
	case 57: //9
		surfaceBezier.controlPoints[selectedControlX][selectedControlY] += ofVec3f(0, 10, 0);
		surfaceBezier.GenerateBezierSurface();
		break;
	}
}

//--------------------------------------------------------------
void SceneSurfaceBezier::keyReleased(int key)
{
}

//--------------------------------------------------------------
void SceneSurfaceBezier::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void SceneSurfaceBezier::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void SceneSurfaceBezier::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void SceneSurfaceBezier::mouseReleased(int x, int y, int button)
{
}


void SceneSurfaceBezier::StartButton()
{
	isPlaying = true;
	surfaceBezier = SurfaceBezier(nbControlsSlider);
}


void SceneSurfaceBezier::UpdateNbPas()
{
	nbPas = nbPasSlider;
	surfaceBezier.nbPas = nbPasSlider;
	surfaceBezier.GenerateBezierSurface();
}